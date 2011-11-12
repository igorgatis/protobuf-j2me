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
import java.util.Vector;

/**
 * This class is used internally by the Protocol Buffer library and generated
 * message implementations. It is public only because those generated messages
 * do not reside in the {@code protobuf} package. Others should not use this
 * class directly.
 * 
 * This class contains constants and helper functions useful for dealing with
 * the Protocol Buffer wire format.
 * 
 * @author kenton@google.com Kenton Varda
 * @author gatis@google.com Igor Gatis
 */
public final class WireFormat {
  // Do not allow instantiation.
  private WireFormat() {
  }

  static final int WIRETYPE_VARINT = 0;
  static final int WIRETYPE_FIXED64 = 1;
  static final int WIRETYPE_LENGTH_DELIMITED = 2;
  static final int WIRETYPE_START_GROUP = 3;
  static final int WIRETYPE_END_GROUP = 4;
  static final int WIRETYPE_FIXED32 = 5;

  static final int TAG_TYPE_BITS = 3;
  static final int TAG_TYPE_MASK = (1 << TAG_TYPE_BITS) - 1;

  /** Given a tag value, determines the wire type (the lower 3 bits). */
  static int getTagWireType(final int tag) {
    return tag & TAG_TYPE_MASK;
  }

  /** Given a tag value, determines the field number (the upper 29 bits). */
  public static int getTagFieldNumber(final int tag) {
    return tag >>> TAG_TYPE_BITS;
  }

  /** Makes a tag value given a field number and wire type. */
  static int makeTag(final int fieldNumber, final int wireType) {
    return (fieldNumber << TAG_TYPE_BITS) | wireType;
  }

  /**
   * Lite equivalent to {@link Descriptors.FieldDescriptor.JavaType}. This is
   * only here to support the lite runtime and should not be used by users.
   */
  public static final class JavaType {
    public static final JavaType INT = new JavaType(new Integer(0));
    public static final JavaType LONG = new JavaType(new Long(0));
    public static final JavaType FLOAT = new JavaType(new Float(0));
    public static final JavaType DOUBLE = new JavaType(new Double(0));
    public static final JavaType BOOLEAN = new JavaType(new Boolean(false));
    public static final JavaType STRING = new JavaType(new String(""));
    public static final JavaType BYTE_STRING = new JavaType(ByteString.EMPTY);
    public static final JavaType ENUM = new JavaType(null);
    public static final JavaType MESSAGE = new JavaType(null);

    JavaType(final Object defaultDefault) {
      this.defaultDefault = defaultDefault;
    }

    /**
     * The default default value for fields of this type, if it's a primitive
     * type.
     */
    Object getDefaultDefault() {
      return defaultDefault;
    }

    private final Object defaultDefault;
  }

  /**
   * Lite equivalent to {@link Descriptors.FieldDescriptor.Type}. This is only
   * here to support the lite runtime and should not be used by users.
   */
  public static class FieldType {
    public static final FieldType BOOL = new FieldType(0, JavaType.BOOLEAN,
        WIRETYPE_VARINT);
    public static final FieldType BYTES = new FieldType(1,
        JavaType.BYTE_STRING, WIRETYPE_LENGTH_DELIMITED);
    public static final FieldType DOUBLE = new FieldType(2, JavaType.DOUBLE,
        WIRETYPE_FIXED64);
    public static final FieldType ENUM = new FieldType(3, JavaType.ENUM,
        WIRETYPE_VARINT);
    public static final FieldType FIXED32 = new FieldType(4, JavaType.INT,
        WIRETYPE_FIXED32);
    public static final FieldType FIXED64 = new FieldType(5, JavaType.LONG,
        WIRETYPE_FIXED64);
    public static final FieldType FLOAT = new FieldType(6, JavaType.FLOAT,
        WIRETYPE_FIXED32);
    public static final FieldType GROUP = new FieldType(7, JavaType.MESSAGE,
        WIRETYPE_START_GROUP);
    public static final FieldType INT32 = new FieldType(8, JavaType.INT,
        WIRETYPE_VARINT);
    public static final FieldType INT64 = new FieldType(9, JavaType.LONG,
        WIRETYPE_VARINT);
    public static final FieldType MESSAGE = new FieldType(10, JavaType.MESSAGE,
        WIRETYPE_LENGTH_DELIMITED);
    public static final FieldType SFIXED32 = new FieldType(11, JavaType.INT,
        WIRETYPE_FIXED32);
    public static final FieldType SFIXED64 = new FieldType(12, JavaType.LONG,
        WIRETYPE_FIXED64);
    public static final FieldType SINT32 = new FieldType(13, JavaType.INT,
        WIRETYPE_VARINT);
    public static final FieldType SINT64 = new FieldType(14, JavaType.LONG,
        WIRETYPE_VARINT);
    public static final FieldType STRING = new FieldType(15, JavaType.STRING,
        WIRETYPE_LENGTH_DELIMITED);
    public static final FieldType UINT32 = new FieldType(16, JavaType.INT,
        WIRETYPE_VARINT);
    public static final FieldType UINT64 = new FieldType(17, JavaType.LONG,
        WIRETYPE_VARINT);

    private FieldType(final int value, final JavaType javaType,
        final int wireType) {
      this.value = value;
      this.javaType = javaType;
      this.wireType = wireType;
    }

    private final int value;
    private final JavaType javaType;
    private final int wireType;

    protected JavaType getJavaType() {
      return javaType;
    }

    protected int getWireType() {
      return wireType;
    }

    protected boolean isPackable() {
      if (this == UINT32 || this == ENUM || this == SFIXED32
          || this == SFIXED64 || this == SINT32 || this == SINT64) {
        return true;
      }
      return false;
    }

    protected int computeListSerializedSize(Extension ext, Vector list) {
      int size = 0;
      // if (ext.getType().isPackable()) {
      if (ext.isPacked()) {
        size += CodedOutputStream.computeTagSize(ext.getNumber());
        int dataSize = 0;
        for (int i = 0; i < list.size(); i++) {
          dataSize += computeSerializedSize(false, ext, list.elementAt(i));
        }
        size += dataSize;
        size += CodedOutputStream.computeRawVarint32Size(dataSize);
      } else {
        for (int i = 0; i < list.size(); i++) {
          size += computeSerializedSize(true, ext, list.elementAt(i));
        }
      }
      return size;
    }

    protected void writeList(Extension ext, Vector list,
        CodedOutputStream output) throws IOException {
      // if (ext.getType().isPackable()) {
      if (ext.isPacked()) {
        output.writeTag(ext.getNumber(), WireFormat.WIRETYPE_LENGTH_DELIMITED);
        // Compute the total data size so the length can be written.
        int dataSize = 0;
        for (int i = 0; i < list.size(); i++) {
          dataSize += computeSerializedSize(false, ext, list.elementAt(i));
        }
        output.writeRawVarint32(dataSize);
        // Write the data itself, without any tags.
        for (int i = 0; i < list.size(); i++) {
          writeElement(false, ext, list.elementAt(i), output);
        }
      } else {
        for (int i = 0; i < list.size(); i++) {
          writeElement(true, ext, list.elementAt(i), output);
        }
      }
    }

    protected int computeSerializedSize(boolean withTag, Extension ext,
        Object obj) {
      int size = withTag ? CodedOutputStream.computeTagSize(ext.getNumber())
          : 0;
      switch (value) {
        case 0: // BOOL.value:
          return size
              + CodedOutputStream.computeBoolSizeNoTag(((Boolean) obj)
                  .booleanValue());
        case 1: // BYTES.value:
          return size
              + CodedOutputStream.computeBytesSizeNoTag((ByteString) obj);
        case 2: // DOUBLE.value:
          return size
              + CodedOutputStream.computeDoubleSizeNoTag(((Double) obj)
                  .doubleValue());
        case 3: // ENUM.value:
          return size
              + CodedOutputStream.computeEnumSizeNoTag(((EnumType) obj)
                  .getValue());
        case 4: // FIXED32.value:
          return size
              + CodedOutputStream.computeFixed32SizeNoTag(((Integer) obj)
                  .intValue());
        case 5: // FIXED64.value:
          return size
              + CodedOutputStream.computeFixed64SizeNoTag(((Long) obj)
                  .longValue());
        case 6: // FLOAT.value:
          return size
              + CodedOutputStream.computeFloatSizeNoTag(((Float) obj)
                  .floatValue());
          // case 7: // GROUP.value:
          // return size +
// CodedOutputStream.computeGroupSizeNoTag((Message)obj);
        case 8: // INT32.value:
          return size
              + CodedOutputStream.computeInt32SizeNoTag(((Integer) obj)
                  .intValue());
        case 9: // INT64.value:
          return size
              + CodedOutputStream.computeInt64SizeNoTag(((Long) obj)
                  .longValue());
        case 10: // MESSAGE.value:
          return size
              + CodedOutputStream.computeMessageSizeNoTag((Message) obj);
        case 11: // SFIXED32.value:
          return size
              + CodedOutputStream.computeSFixed32SizeNoTag(((Integer) obj)
                  .intValue());
        case 12: // SFIXED64.value:
          return size
              + CodedOutputStream.computeSFixed64SizeNoTag(((Long) obj)
                  .longValue());
        case 13: // SINT32.value:
          return size
              + CodedOutputStream.computeSInt32SizeNoTag(((Integer) obj)
                  .intValue());
        case 14: // SINT64.value:
          return size
              + CodedOutputStream.computeSInt64SizeNoTag(((Long) obj)
                  .longValue());
        case 15: // STRING.value:
          return size + CodedOutputStream.computeStringSizeNoTag((String) obj);
        case 16: // UINT32.value:
          return size
              + CodedOutputStream.computeUInt32SizeNoTag(((Integer) obj)
                  .intValue());
        case 17: // UINT64.value:
          return size
              + CodedOutputStream.computeUInt64SizeNoTag(((Long) obj)
                  .longValue());
      }
      throw new RuntimeException("Should never reach this point.");
    }

    protected void writeElement(boolean withTag, Extension ext, Object obj,
        CodedOutputStream output) throws IOException {
      if (withTag) {
        output.writeTag(ext.getNumber(), this.wireType);
      }
      switch (value) {
        case 0: // BOOL.value:
          output.writeBoolNoTag(((Boolean) obj).booleanValue());
          return;
        case 1: // BYTES.value:
          output.writeBytesNoTag((ByteString) obj);
          return;
        case 2: // DOUBLE.value:
          output.writeDoubleNoTag(((Double) obj).doubleValue());
          return;
        case 3: // ENUM.value:
          output.writeEnumNoTag(((EnumType) obj).getValue());
          return;
        case 4: // FIXED32.value:
          output.writeFixed32NoTag(((Integer) obj).intValue());
          return;
        case 5: // FIXED64.value:
          output.writeFixed64NoTag(((Long) obj).longValue());
          return;
        case 6: // FLOAT.value:
          output.writeFloatNoTag(((Float) obj).floatValue());
          return;
          // case 7: // GROUP.value:
          // output.writeGroupNoTag((Message)obj); return;
        case 8: // INT32.value:
          output.writeInt32NoTag(((Integer) obj).intValue());
          return;
        case 9: // INT64.value:
          output.writeInt64NoTag(((Long) obj).longValue());
          return;
        case 10: // MESSAGE.value:
          output.writeMessageNoTag((Message) obj);
          return;
        case 11: // SFIXED32.value:
          output.writeSFixed32NoTag(((Integer) obj).intValue());
          return;
        case 12: // SFIXED64.value:
          output.writeSFixed64NoTag(((Long) obj).longValue());
          return;
        case 13: // SINT32.value:
          output.writeSInt32NoTag(((Integer) obj).intValue());
          return;
        case 14: // SINT64.value:
          output.writeSInt64NoTag(((Long) obj).longValue());
          return;
        case 15: // STRING.value:
          output.writeStringNoTag((String) obj);
          return;
        case 16: // UINT32.value:
          output.writeUInt32NoTag(((Integer) obj).intValue());
          return;
        case 17: // UINT64.value:
          output.writeUInt64NoTag(((Long) obj).longValue());
          return;
      }
      throw new RuntimeException("Should never reach this point.");
    }

    public Object readPrimitiveField(CodedInputStream input) throws IOException {
      switch (value) {
        case 0: // BOOL
          return new Boolean(input.readBool());
        case 1: // BYTES:
          return input.readBytes();
        case 2: // DOUBLE:
          return new Double(input.readDouble());
        case 3: // ENUM:
          // We don't handle enums because we don't know what to do if
          // the value is not recognized.
          throw new IllegalArgumentException(
              "readPrimitiveField() cannot handle enums.");
        case 4: // FIXED32:
          return new Integer(input.readFixed32());
        case 5: // FIXED64:
          return new Long(input.readFixed64());
        case 6: // FLOAT:
          return new Float(input.readFloat());
        case 7: // GROUP:
          throw new IllegalArgumentException(
              "readPrimitiveField() cannot handle nested groups.");
        case 8: // INT32:
          return new Integer(input.readInt32());
        case 9: // INT64:
          return new Long(input.readInt64());
        case 10: // MESSAGE:
          throw new IllegalArgumentException(
              "readPrimitiveField() cannot handle embedded messages.");
        case 11: // SFIXED32:
          return new Integer(input.readSFixed32());
        case 12: // SFIXED64:
          return new Long(input.readSFixed64());
        case 13: // SINT32:
          return new Integer(input.readSInt32());
        case 14: // SINT64:
          return new Long(input.readSInt64());
        case 15: // STRING:
          return input.readString();
        case 16: // UINT32:
          return new Integer(input.readUInt32());
        case 17: // UINT64:
          return new Long(input.readUInt64());
      }
      throw new RuntimeException(
          "There is no way to get here, but the compiler thinks otherwise.");
    }
  }

  // Field numbers for feilds in MessageSet wire format.
  // static final int MESSAGE_SET_ITEM = 1;
  // static final int MESSAGE_SET_TYPE_ID = 2;
  // static final int MESSAGE_SET_MESSAGE = 3;

  // Tag numbers.
  // static final int MESSAGE_SET_ITEM_TAG =
  // makeTag(MESSAGE_SET_ITEM, WIRETYPE_START_GROUP);
  // static final int MESSAGE_SET_ITEM_END_TAG =
  // makeTag(MESSAGE_SET_ITEM, WIRETYPE_END_GROUP);
  // static final int MESSAGE_SET_TYPE_ID_TAG = makeTag(MESSAGE_SET_TYPE_ID,
  // WIRETYPE_VARINT);
  // static final int MESSAGE_SET_MESSAGE_TAG = makeTag(MESSAGE_SET_MESSAGE,
  // WIRETYPE_LENGTH_DELIMITED);
}
