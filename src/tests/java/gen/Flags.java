// Generated by the protocol buffer compiler.  DO NOT EDIT!

public enum Flags
    implements com.google.protobuf.ProtocolMessageEnum {
  VAL0(0, 0),
  VAL1(1, 1),
  VAL2(2, 2),
  ;
  
  public static final int VAL0_VALUE = 0;
  public static final int VAL1_VALUE = 1;
  public static final int VAL2_VALUE = 2;
  
  
  public final int getNumber() { return value; }
  
  public static Flags valueOf(int value) {
    switch (value) {
      case 0: return VAL0;
      case 1: return VAL1;
      case 2: return VAL2;
      default: return null;
    }
  }
  
  public static com.google.protobuf.Internal.EnumLiteMap<Flags>
      internalGetValueMap() {
    return internalValueMap;
  }
  private static com.google.protobuf.Internal.EnumLiteMap<Flags>
      internalValueMap =
        new com.google.protobuf.Internal.EnumLiteMap<Flags>() {
          public Flags findValueByNumber(int number) {
            return Flags.valueOf(number);
          }
        };
  
  public final com.google.protobuf.Descriptors.EnumValueDescriptor
      getValueDescriptor() {
    return getDescriptor().getValues().get(index);
  }
  public final com.google.protobuf.Descriptors.EnumDescriptor
      getDescriptorForType() {
    return getDescriptor();
  }
  public static final com.google.protobuf.Descriptors.EnumDescriptor
      getDescriptor() {
    return HelloWorld.getDescriptor().getEnumTypes().get(0);
  }
  
  private static final Flags[] VALUES = {
    VAL0, VAL1, VAL2, 
  };
  
  public static Flags valueOf(
      com.google.protobuf.Descriptors.EnumValueDescriptor desc) {
    if (desc.getType() != getDescriptor()) {
      throw new java.lang.IllegalArgumentException(
        "EnumValueDescriptor is not for this type.");
    }
    return VALUES[desc.getIndex()];
  }
  
  private final int index;
  private final int value;
  
  private Flags(int index, int value) {
    this.index = index;
    this.value = value;
  }
  
  // @@protoc_insertion_point(enum_scope:Flags)
}

