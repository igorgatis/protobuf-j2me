// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc. All rights reserved.
// http://code.google.com/p/protobuf-j2me/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

package com.google.protobuf;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

import com.google.protobuf.WireFormat.FieldType;

/**
 * Abstract interface implemented by Protocol Message objects.
 * 
 * @author kenton@google.com Kenton Varda
 * @author gatis@google.com Igor Gatis
 */
public abstract class Message {

  private String name;
  private boolean readOnly;
  private Hashtable extensions;

  protected Message(boolean noInit) {
    this.readOnly = true;
  }

  public Message(String messageName) {
    this.name = messageName;
  }

  protected void assertNotReadOnly() {
    if (readOnly) {
      throw new RuntimeException("Read only message!");
    }
  }

  /**
   * Parses a message of this type from the input and merges it with this
   * message.
   * 
   * Warning: This does not verify that all required fields are present in the
   * input message.
   */
  public abstract void mergeFrom(CodedInputStream input) throws IOException;

  public void mergeFrom(InputStream input) throws IOException {
    mergeFrom(CodedInputStream.newInstance(input));
  }

  public void mergeFrom(ByteString data) throws IOException {
    mergeFrom(data.newCodedInput());
  }

  /**
   * Serializes the message and writes it to {@code output}. This does not flush
   * or close the stream.
   */
  protected abstract void writeTo(CodedOutputStream output) throws IOException;

  public void writeTo(OutputStream output) throws IOException {
    CodedOutputStream codedOut = CodedOutputStream.newInstance(output);
    writeTo(codedOut);
    codedOut.flush();
  }

  /**
   * Returns true if all required fields in the message and all embedded
   * messages are set, false otherwise.
   */
  public abstract boolean isInitialized();

  /**
   * Get the number of bytes required to encode this message. The result is only
   * computed on the first call and memoized after that.
   */
  public abstract int getSerializedSize();

  /**
   * Compares the specified object with this message for equality. Returns
   * <tt>true</tt> if the given object is a message of the same type (as defined
   * by {@code getDescriptorForType()}) and has identical values for all of its
   * fields.
   * 
   * @param other
   *          object to be compared for equality with this message
   * @return <tt>true</tt> if the specified object is equal to this message
   */
  public abstract boolean equals(Object msg);

  /**
   * Returns the hash code value for this message. The hash code of a message is
   * defined to be <tt>getDescriptor().hashCode() ^ map.hashCode()</tt>, where
   * <tt>map</tt> is a map of field numbers to field values.
   * 
   * @return the hash code value for this message
   * @see Map#hashCode()
   */
  public abstract int hashCode();

  /**
   * Converts the message to a string in protocol buffer text format.
   */
  // public abstract String toString();

  /** Returns a copy of underlying message. */
  public abstract Message newInstance();

  /**
   * Serializes the message to a {@code ByteString} and returns it. This is just
   * a trivial wrapper around {@link #writeTo(CodedOutputStream)}.
   */
  public ByteString toByteString() {
    byte[] data = new byte[getSerializedSize()];
    CodedOutputStream output = CodedOutputStream.newInstance(data);
    try {
      writeTo(output);
      output.flush();
    } catch (IOException e) {
      throw new RuntimeException(e.getMessage());
    }
    return new ByteString(data);
  }

  private static int getWireFormatForFieldType(final WireFormat.FieldType type,
      boolean isPacked) {
    if (isPacked) {
      return WireFormat.WIRETYPE_LENGTH_DELIMITED;
    } else {
      return type.getWireType();
    }
  }

  protected boolean parseUnknownField(final CodedInputStream input,
      final int tag) throws IOException {
    return input.skipField(tag);
  }

  /**
   * Called by subclasses to parse an unknown field or an extension.
   * 
   * @return {@code true} unless the tag is an end-group tag.
   */
  protected boolean parseUnknownFieldWithExtensions(
      final CodedInputStream input, final int tag) throws IOException {
    boolean unknown = false;
    boolean packed = false;
    final int wireType = WireFormat.getTagWireType(tag);
    final int fieldNumber = WireFormat.getTagFieldNumber(tag);
    Extension ext = Extension.getExtensionByNumber(getMessageName(),
        fieldNumber);
    if (ext == null) {
      unknown = true; // Unknown field.
    } else if (wireType == getWireFormatForFieldType(ext.getType(), false /* isPacked */)) {
      packed = false; // Normal, unpacked value.
    } else if (ext.isRepeated()
        && ext.getType().isPackable()
        && wireType == getWireFormatForFieldType(ext.getType(), true /* isPacked */)) {
      packed = true; // Packed value.
    } else {
      unknown = true; // Wrong wire type.
    }
    if (unknown) { // Unknown field or wrong wire type. Skip.
      return input.skipField(tag);
    }

    if (packed) {
      final int length = input.readRawVarint32();
      final int limit = input.pushLimit(length);
      if (ext.getType() == WireFormat.FieldType.ENUM) {
        while (input.getBytesUntilLimit() > 0) {
          final int rawValue = input.readEnum();
          final Object value = ((EnumType) ext.getDefaultValue())
              .getEnum(rawValue);
          if (value == null) {
            // If the number isn't recognized as a valid value for this
            // enum, drop it (don't even add it to unknownFields).
            return true;
          }
          addExtension(ext, value);
        }
      } else {
        while (input.getBytesUntilLimit() > 0) {
          final Object value = ext.getType().readPrimitiveField(input);
          addExtension(ext, value);
        }
      }
      input.popLimit(limit);
    } else {
      final Object value;
      if (ext.getType() == FieldType.MESSAGE) {
        Message subBuilder = null;
        if (!ext.isRepeated()) {
          Message existingValue = (Message) getExtension(ext);
          if (existingValue != null && !existingValue.readOnly) {
            subBuilder = existingValue;
          }
        }
        if (subBuilder == null) {
          subBuilder = ((Message) ext.getDefaultValue()).newInstance();
        }
        if (ext.getType() == WireFormat.FieldType.GROUP) {
          // input.readGroup(ext.getNumber(), subBuilder);
          return input.skipField(tag);
        } else {
          subBuilder.mergeFrom(input);
        }
        value = subBuilder;
      } else if (ext.getType() == FieldType.ENUM) {
        final int rawValue = input.readEnum();
        value = ((EnumType) ext.getDefaultValue()).getEnum(rawValue);
        // If the number isn't recognized as a valid value for this enum,
        // drop it.
        if (value == null) {
          return true;
        }
      } else {
        value = ext.getType().readPrimitiveField(input);
      }
      if (ext.isRepeated()) {
        addExtension(ext, value);
      } else {
        setExtension(ext, value);
      }
    }
    return true;
  }

  // -----------------------------------------------------------------------
  // Extension methods.
  // -----------------------------------------------------------------------

  protected void initExtensionSupport() {
    this.extensions = new Hashtable();
  }

  protected String getMessageName() {
    return this.name;
  }

  // Single value.

  private void checkNonRepeatedExtension(Extension ext) {
    if (ext.isRepeated()) {
      throw new RuntimeException("This is a repeated extension.");
    }
  }

  public boolean hasExtension(Extension ext) {
    Extension.checkExtensionSupport(getMessageName(), ext);
    checkNonRepeatedExtension(ext);
    return this.extensions.contains(ext);
  }

  public Object getExtension(Extension ext) {
    Extension.checkExtensionSupport(getMessageName(), ext);
    checkNonRepeatedExtension(ext);
    Object value = this.extensions.get(ext);
    if (value != null) {
      return value;
    }
    return ext.getDefaultValue();
  }

  public void setExtension(Extension ext, Object obj) {
    assertNotReadOnly();
    Extension.checkExtensionSupport(getMessageName(), ext);
    checkNonRepeatedExtension(ext);
    this.extensions.put(ext, obj);
  }

  // Repeated values.

  private Vector getOrCreateList(Extension ext) {
    Extension.checkExtensionSupport(getMessageName(), ext);
    if (!ext.isRepeated()) {
      throw new RuntimeException("Not a repeated extension.");
    }
    Vector list = (Vector) this.extensions.get(ext);
    if (list == null) {
      list = new Vector();
      this.extensions.put(ext, list);
    }
    return list;
  }

  public Enumeration getExtensionList(Extension ext) {
    Vector list = getOrCreateList(ext);
    return list.elements();
  }

  public int getExtensionCount(Extension ext) {
    Vector list = getOrCreateList(ext);
    return list.size();
  }

  public Object getExtension(Extension ext, int index) {
    Vector list = getOrCreateList(ext);
    return list.elementAt(index);
  }

  public void setExtension(Extension ext, int index, Object obj) {
    assertNotReadOnly();
    Vector list = getOrCreateList(ext);
    list.setElementAt(obj, index);
  }

  public void addExtension(Extension ext, Object obj) {
    assertNotReadOnly();
    Vector list = getOrCreateList(ext);
    list.addElement(obj);
  }

  public void clearExtension(Extension ext) {
    assertNotReadOnly();
    Vector list = getOrCreateList(ext);
    list.removeAllElements();
  }

  // -----------------------------------------------------------------------
  // Extension methods - Internal Only.
  // -----------------------------------------------------------------------

  protected boolean extensionsEquals(Message msg) {
    if (getMessageName().equals(msg.getMessageName()) == false) {
      throw new RuntimeException("Type mismtach.");
    }
    Enumeration supportedExtensions = Extension
        .getSupportedExtensions(getMessageName());
    while (supportedExtensions.hasMoreElements()) {
      Extension ext = (Extension) supportedExtensions.nextElement();
      Object localValue = this.extensions.get(ext);
      Object otherValue = msg.extensions.get(ext);
      if (localValue != null && otherValue != null) {
        if (localValue.equals(otherValue) == false) {
          return false;
        }
      } else if (localValue != null || otherValue != null) {
        return false;
      }
    }
    return true;
  }

  protected int extensionsHashCode() {
    int hash = 0;
    Enumeration supportedExtensions = Extension
        .getSupportedExtensions(getMessageName());
    while (supportedExtensions.hasMoreElements()) {
      Extension ext = (Extension) supportedExtensions.nextElement();
      if (ext.isRepeated()) {
        Vector list = (Vector) this.extensions.get(ext);
        for (int i = 0; i < list.size(); i++) {
          hash += 35 * list.elementAt(i).hashCode();
        }
      } else {
        // hasExtension()
        Object value = this.extensions.get(ext);
        if (value != null) {
          hash += 35 * value.hashCode();
        }
      }
    }
    return hash;
  }

  protected boolean extensionsAreInitialized() {
    Enumeration supportedExtensions = Extension
        .getSupportedExtensions(getMessageName());
    while (supportedExtensions.hasMoreElements()) {
      Extension ext = (Extension) supportedExtensions.nextElement();
      // Check that all required fields are present.
      if (ext.isRequired() && hasExtension(ext) == false) {
        return false;
      }
      // Check that embedded messages are initialized.
      if (ext.getType() == WireFormat.FieldType.MESSAGE) {
        if (ext.isRepeated()) {
          Enumeration repMessagesEnum = getExtensionList(ext);
          while (repMessagesEnum.hasMoreElements()) {
            Message msg = (Message) repMessagesEnum.nextElement();
            if (msg.isInitialized() == false) {
              return false;
            }
          }
        } else {
          // hasExtension()
          Message msg = (Message) this.extensions.get(ext);
          if (msg != null) {
            if (msg.isInitialized() == false) {
              return false;
            }
          }
        }
      }
    }
    return true;
  }

  protected void writeExtensions(CodedOutputStream output)
      throws IOException {
    Enumeration keys = this.extensions.keys();
    while (keys.hasMoreElements()) {
      Extension ext = (Extension) keys.nextElement();
      if (ext.isRepeated()) {
        Vector list = (Vector) this.extensions.get(ext);
        ext.getType().writeList(ext, list, output);
      } else {
        // hasExtension()
        Object value = this.extensions.get(ext);
        if (value != null) {
          ext.getType().writeElement(true, ext, value, output);
        }
      }
    }
//    Enumeration supportedExtensions = Extension
//        .getSupportedExtensions(getMessageName());
//    while (supportedExtensions.hasMoreElements()) {
//      Extension ext = (Extension) supportedExtensions.nextElement();
//      if (ext.getNumber() >= number) {
//        continue;
//      }
//      if (ext.isRepeated()) {
//        Vector list = (Vector) this.extensions.get(ext);
//        ext.getType().writeList(ext, list, output);
//      } else {
//        // hasExtension()
//        Object value = this.extensions.get(ext);
//        if (value != null) {
//          ext.getType().writeElement(true, ext, value, output);
//        }
//      }
//    }
  }

  protected int extensionsSerializedSize() {
    Enumeration supportedExtensions = Extension
        .getSupportedExtensions(getMessageName());
    int size = 0;
    while (supportedExtensions.hasMoreElements()) {
      Extension ext = (Extension) supportedExtensions.nextElement();
      if (ext.isRepeated()) {
        Vector list = getOrCreateList(ext);
        size += ext.getType().computeListSerializedSize(ext, list);
      } else {
        // hasExtension()
        Object value = this.extensions.get(ext);
        if (value != null) {
          size += ext.getType().computeSerializedSize(true, ext, value);
        }
      }
    }
    return size;
  }

  public static interface Builder {
    public void mergeFrom(CodedInputStream input) throws IOException;

    public Message build();
  }
}
