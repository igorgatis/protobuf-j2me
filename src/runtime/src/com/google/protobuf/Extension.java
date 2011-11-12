// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc. All rights reserved.
// http://code.google.com/p/protobuf/
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

import java.util.Enumeration;
import java.util.Hashtable;

import com.google.protobuf.WireFormat.FieldType;

/**
 * Super class for extension types.
 * 
 * @author gatis@google.com
 * 
 */
public class Extension {

  private static Hashtable registry = new Hashtable();

  public static void register(Extension ext) {
    String message = ext.getExtendedMessage();
    Hashtable hash = (Hashtable) registry.get(message);
    if (hash == null) {
      hash = new Hashtable();
      registry.put(message, hash);
    }
    hash.put(new Integer(ext.getNumber()), ext);
  }

  private static Hashtable getRegistry(String message) {
    Hashtable hash = (Hashtable) registry.get(message);
    if (hash == null) {
      throw new RuntimeException("Extensions not supported by " + message + ".");
    }
    return hash;
  }

  public static void checkExtensionSupport(String message, Extension ext) {
    Hashtable set = getRegistry(message);
    Integer key = new Integer(ext.getNumber());
    if (ext != null && set.containsKey(key) == false) {
      throw new RuntimeException("Extension " + ext + " not supported by "
          + message + ".");
    }
  }

  public static Enumeration getSupportedExtensions(String message) {
    return getRegistry(message).elements();
  }

  public static Extension getExtensionByNumber(String message, int number) {
    Hashtable map = getRegistry(message);
    return (Extension) map.get(new Integer(number));
  }

  public static Extension newExtension(String classname, FieldType type,
      int number, boolean required, Object defaultValue) {
    byte flags = required ? IS_REQUIRED : 0;
    return new Extension(classname, type, number, flags, defaultValue);
  }

  public static Extension newRepeatedExtension(String classname,
      FieldType type, int number, boolean isPacked, Object defaultValue) {
    byte flags = IS_REPEATED;
    flags |= isPacked ? IS_PACKED : 0;
    return new Extension(classname, type, number, flags, defaultValue);
  }

  public static final byte IS_REQUIRED = 0x01;
  public static final byte IS_REPEATED = 0x02;
  public static final byte IS_PACKED = 0x04;

  private String extendedMessage;
  private FieldType type;
  private byte flags;
  private int number;
  private Object defaultValue;

  private Extension(String extendedMessage, FieldType type, int number,
      byte flags, Object defaultValue) {
    this.extendedMessage = extendedMessage;
    this.type = type;
    this.number = number;
    this.flags = flags;
    this.defaultValue = defaultValue;
  }

  public String getExtendedMessage() {
    return extendedMessage;
  }

  public FieldType getType() {
    return type;
  }

  public int getNumber() {
    return number;
  }

  public boolean isRequired() {
    return (flags & IS_REQUIRED) != 0;
  }

  public boolean isRepeated() {
    return (flags & IS_REPEATED) != 0;
  }

  public boolean isPacked() {
    return (flags & IS_PACKED) != 0;
  }

  public Object getDefaultValue() {
    return defaultValue;
  }

  public int hashCode() {
    return this.extendedMessage.hashCode() ^ this.number;
  }

  public boolean equals(Object obj) {
    if (obj == this) {
      return true;
    }
    if (obj instanceof Extension) {
      Extension ext = (Extension) obj;
      return number == ext.number
          && extendedMessage.equals(ext.extendedMessage);
    }
    return false;
  }

  public String toString() {
    String def = this.defaultValue != null ? this.defaultValue.getClass()
        .getName() : "null";
    return this.extendedMessage + ":" + this.number + "[" + def + "]";
  }
}
