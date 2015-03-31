// Generated by the protocol buffer compiler.  DO NOT EDIT!

public  final class Foo extends
    com.google.protobuf.Message {
  private int set_mask_0_;
  public Foo() {
    super("Foo");
    initFields();
  }
  private Foo(boolean noInit) { super(true); }
  
  private static final Foo defaultInstance;
  public static Foo getDefaultInstance() {
    return defaultInstance;
  }
  
  public Foo getDefaultInstanceForType() {
    return defaultInstance;
  }
  
  // optional string opt_str = 1;
  private java.lang.String optStr_ = "";
  public java.lang.String getOptStr() { return optStr_; }
  public boolean hasOptStr() { return (set_mask_0_ & 1) != 0; }
  public void clearOptStr() {
    assertNotReadOnly();
    set_mask_0_ &= ~1;
    optStr_ = "";
  }
  public void setOptStr(java.lang.String value) {
    assertNotReadOnly();
    set_mask_0_ |= 1;
    optStr_ = value;
  }
  
  private void initFields() {
    initExtensionSupport();
  }
  public final boolean isInitialized() {
    if (!extensionsAreInitialized()) return false;
    return true;
  }
  
  public boolean equals(Object obj) {
    if (this == obj) return true;
    if (!(obj instanceof Foo)) return false;
    Foo msg = (Foo)obj;
    if (!optStr_.equals(msg.optStr_)) {
      return false;
    }
    if (!extensionsEquals(msg)) return false;
    return true;
  }
  
  public int hashCode() {
    int hash = 41 * getClass().getName().hashCode();
    if (hasOptStr()) hash += 31 * optStr_.hashCode();
    hash += extensionsHashCode();
    return hash;
  }
  
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    if (hasOptStr()) {
      output.writeString(1, getOptStr());
    }
    writeExtensions(output);
  }
  
  public int getSerializedSize() {
    int size = 0;
    if (hasOptStr()) {
      size += com.google.protobuf.CodedOutputStream
        .computeStringSize(1, getOptStr());
    }
    size += extensionsSerializedSize();
    return size;
  }
  
  public static Foo parseFrom(
      java.io.InputStream input)
      throws java.io.IOException {
    com.google.protobuf.CodedInputStream codedInput =
        com.google.protobuf.CodedInputStream.newInstance(input);
    return parseFrom(codedInput);
  }
  
  public static Foo parseFrom(
      com.google.protobuf.CodedInputStream codedInput)
      throws java.io.IOException {
    Foo proto = new Foo();
    proto.mergeFrom(codedInput);
    return proto;
  }
  
  public com.google.protobuf.Message newInstance() {
    return new Foo();
  }
  
  public void mergeFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    assertNotReadOnly();
    while (true) {
      int tag = input.readTag();
      switch (tag) {
        case 0:
          return;
        default: {
          if (!parseUnknownFieldWithExtensions(input, tag)) {
            return;
          }
          break;
        }
        case 10: {
          setOptStr(input.readString());
          break;
        }
      }
    }
  }
  
  
  static {
    defaultInstance = new Foo(true);
    HelloWorld.internalForceInit();
    defaultInstance.initFields();
  }
  
  // @@protoc_insertion_point(class_scope:Foo)
}
