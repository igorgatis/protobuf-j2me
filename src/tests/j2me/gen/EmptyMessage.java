// Generated by the protocol buffer compiler.  DO NOT EDIT!

public  final class EmptyMessage extends
    com.google.protobuf.Message {
  public EmptyMessage() {
    super("EmptyMessage");
    initFields();
  }
  private EmptyMessage(boolean noInit) { super(true); }
  
  private static final EmptyMessage defaultInstance;
  public static EmptyMessage getDefaultInstance() {
    return defaultInstance;
  }
  
  public EmptyMessage getDefaultInstanceForType() {
    return defaultInstance;
  }
  
  private void initFields() {
  }
  public final boolean isInitialized() {
    return true;
  }
  
  public boolean equals(Object obj) {
    if (this == obj) return true;
    if (!(obj instanceof EmptyMessage)) return false;
    EmptyMessage msg = (EmptyMessage)obj;
    return true;
  }
  
  public int hashCode() {
    int hash = 41 * getClass().getName().hashCode();
    return hash;
  }
  
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
  }
  
  public int getSerializedSize() {
    int size = 0;
    return size;
  }
  
  public static EmptyMessage parseFrom(
      java.io.InputStream input)
      throws java.io.IOException {
    com.google.protobuf.CodedInputStream codedInput =
        com.google.protobuf.CodedInputStream.newInstance(input);
    return parseFrom(codedInput);
  }
  
  public static EmptyMessage parseFrom(
      com.google.protobuf.CodedInputStream codedInput)
      throws java.io.IOException {
    EmptyMessage proto = new EmptyMessage();
    proto.mergeFrom(codedInput);
    return proto;
  }
  
  public com.google.protobuf.Message newInstance() {
    return new EmptyMessage();
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
          if (!parseUnknownField(input, tag)) {
            return;
          }
          break;
        }
      }
    }
  }
  
  
  static {
    defaultInstance = new EmptyMessage(true);
    HelloWorld.internalForceInit();
    defaultInstance.initFields();
  }
  
  // @@protoc_insertion_point(class_scope:EmptyMessage)
}
