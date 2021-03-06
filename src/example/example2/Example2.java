// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: example2.proto

public final class Example2 {
  private Example2() {}
  // AnotherCStyleEnum enum.
  public static final int VAL1 = 1;
  public static final int VAL2 = 2;
  
  public  static final class MsgNoBuilder extends
      com.google.protobuf.Message {
    public MsgNoBuilder() {
      super("MsgNoBuilder");
      initFields();
    }
    private MsgNoBuilder(boolean noInit) { super(true); }
    
    private static final MsgNoBuilder defaultInstance;
    public static MsgNoBuilder getDefaultInstance() {
      return defaultInstance;
    }
    
    public MsgNoBuilder getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    private void initFields() {
    }
    public final boolean isInitialized() {
      return true;
    }
    
    public boolean equals(Object obj) {
      if (this == obj) return true;
      if (!(obj instanceof Example2.MsgNoBuilder)) return false;
      Example2.MsgNoBuilder msg = (Example2.MsgNoBuilder)obj;
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
    
    public static Example2.MsgNoBuilder parseFrom(
        java.io.InputStream input)
        throws java.io.IOException {
      com.google.protobuf.CodedInputStream codedInput =
          com.google.protobuf.CodedInputStream.newInstance(input);
      return parseFrom(codedInput);
    }
    
    public static Example2.MsgNoBuilder parseFrom(
        com.google.protobuf.CodedInputStream codedInput)
        throws java.io.IOException {
      Example2.MsgNoBuilder proto = new Example2.MsgNoBuilder();
      proto.mergeFrom(codedInput);
      return proto;
    }
    
    public com.google.protobuf.Message newInstance() {
      return new Example2.MsgNoBuilder();
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
      defaultInstance = new MsgNoBuilder(true);
      Example2.internalForceInit();
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:MsgNoBuilder)
  }
  
  public  static final class MsgKeepUnknowns extends
      com.google.protobuf.Message {
    public MsgKeepUnknowns() {
      super("MsgKeepUnknowns");
      initFields();
    }
    private MsgKeepUnknowns(boolean noInit) { super(true); }
    
    private static final MsgKeepUnknowns defaultInstance;
    public static MsgKeepUnknowns getDefaultInstance() {
      return defaultInstance;
    }
    
    public MsgKeepUnknowns getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    private void initFields() {
    }
    public final boolean isInitialized() {
      return true;
    }
    
    public boolean equals(Object obj) {
      if (this == obj) return true;
      if (!(obj instanceof Example2.MsgKeepUnknowns)) return false;
      Example2.MsgKeepUnknowns msg = (Example2.MsgKeepUnknowns)obj;
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
    
    public static Example2.MsgKeepUnknowns parseFrom(
        java.io.InputStream input)
        throws java.io.IOException {
      com.google.protobuf.CodedInputStream codedInput =
          com.google.protobuf.CodedInputStream.newInstance(input);
      return parseFrom(codedInput);
    }
    
    public static Example2.MsgKeepUnknowns parseFrom(
        com.google.protobuf.CodedInputStream codedInput)
        throws java.io.IOException {
      Example2.MsgKeepUnknowns proto = new Example2.MsgKeepUnknowns();
      proto.mergeFrom(codedInput);
      return proto;
    }
    
    public com.google.protobuf.Message newInstance() {
      return new Example2.MsgKeepUnknowns();
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
      defaultInstance = new MsgKeepUnknowns(true);
      Example2.internalForceInit();
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:MsgKeepUnknowns)
  }
  
  public  static final class MsgWithCStyleEnum extends
      com.google.protobuf.Message {
    private int set_mask_0_;
    public MsgWithCStyleEnum() {
      super("MsgWithCStyleEnum");
      initFields();
    }
    private MsgWithCStyleEnum(boolean noInit) { super(true); }
    
    private static final MsgWithCStyleEnum defaultInstance;
    public static MsgWithCStyleEnum getDefaultInstance() {
      return defaultInstance;
    }
    
    public MsgWithCStyleEnum getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    // CStyleEnum enum.
    public static final int VAL1 = 1;
    public static final int VAL2 = 2;
    
    // optional .MsgWithCStyleEnum.CStyleEnum my_enum = 1 [default = VAL2];
    private int myEnum_;
    public int getMyEnum() { return myEnum_; }
    public boolean hasMyEnum() { return (set_mask_0_ & 1) != 0; }
    public void clearMyEnum() {
      assertNotReadOnly();
      set_mask_0_ &= ~1;
      myEnum_ = Example2.MsgWithCStyleEnum.VAL2;
    }
    public void setMyEnum(int value) {
      assertNotReadOnly();
      set_mask_0_ |= 1;
      myEnum_ = value;
    }
    
    private void initFields() {
      myEnum_ = Example2.MsgWithCStyleEnum.VAL2;
    }
    public final boolean isInitialized() {
      return true;
    }
    
    public boolean equals(Object obj) {
      if (this == obj) return true;
      if (!(obj instanceof Example2.MsgWithCStyleEnum)) return false;
      Example2.MsgWithCStyleEnum msg = (Example2.MsgWithCStyleEnum)obj;
      if (myEnum_ != msg.myEnum_) {
        return false;
      }
      return true;
    }
    
    public int hashCode() {
      int hash = 41 * getClass().getName().hashCode();
      if (hasMyEnum())  hash += 37 * myEnum_;
      return hash;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      if (hasMyEnum()) {
        output.writeEnum(1, getMyEnum());
      }
    }
    
    public int getSerializedSize() {
      int size = 0;
      if (hasMyEnum()) {
        size += com.google.protobuf.CodedOutputStream
          .computeEnumSize(1, getMyEnum());
      }
      return size;
    }
    
    public static Example2.MsgWithCStyleEnum parseFrom(
        java.io.InputStream input)
        throws java.io.IOException {
      com.google.protobuf.CodedInputStream codedInput =
          com.google.protobuf.CodedInputStream.newInstance(input);
      return parseFrom(codedInput);
    }
    
    public static Example2.MsgWithCStyleEnum parseFrom(
        com.google.protobuf.CodedInputStream codedInput)
        throws java.io.IOException {
      Example2.MsgWithCStyleEnum proto = new Example2.MsgWithCStyleEnum();
      proto.mergeFrom(codedInput);
      return proto;
    }
    
    public com.google.protobuf.Message newInstance() {
      return new Example2.MsgWithCStyleEnum();
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
          case 8: {
            setMyEnum(input.readEnum());
            break;
          }
        }
      }
    }
    
    
    static {
      defaultInstance = new MsgWithCStyleEnum(true);
      Example2.internalForceInit();
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:MsgWithCStyleEnum)
  }
  
  
  static {
  }
  
  public static void internalForceInit() {}
  
  // @@protoc_insertion_point(outer_class_scope)
}
