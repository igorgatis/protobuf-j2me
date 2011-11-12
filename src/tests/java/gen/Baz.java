// Generated by the protocol buffer compiler.  DO NOT EDIT!

public  final class Baz extends
    com.google.protobuf.GeneratedMessage
    implements BazOrBuilder {
  // Use Baz.newBuilder() to construct.
  private Baz(Builder builder) {
    super(builder);
  }
  private Baz(boolean noInit) {}
  
  private static final Baz defaultInstance;
  public static Baz getDefaultInstance() {
    return defaultInstance;
  }
  
  public Baz getDefaultInstanceForType() {
    return defaultInstance;
  }
  
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return HelloWorld.internal_static_Baz_descriptor;
  }
  
  protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return HelloWorld.internal_static_Baz_fieldAccessorTable;
  }
  
  private void initFields() {
  }
  private byte memoizedIsInitialized = -1;
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized != -1) return isInitialized == 1;
    
    memoizedIsInitialized = 1;
    return true;
  }
  
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    getSerializedSize();
    getUnknownFields().writeTo(output);
  }
  
  private int memoizedSerializedSize = -1;
  public int getSerializedSize() {
    int size = memoizedSerializedSize;
    if (size != -1) return size;
  
    size = 0;
    size += getUnknownFields().getSerializedSize();
    memoizedSerializedSize = size;
    return size;
  }
  
  private static final long serialVersionUID = 0L;
  @java.lang.Override
  protected java.lang.Object writeReplace()
      throws java.io.ObjectStreamException {
    return super.writeReplace();
  }
  
  public static Baz parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data).buildParsed();
  }
  public static Baz parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data, extensionRegistry)
             .buildParsed();
  }
  public static Baz parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data).buildParsed();
  }
  public static Baz parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data, extensionRegistry)
             .buildParsed();
  }
  public static Baz parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input).buildParsed();
  }
  public static Baz parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input, extensionRegistry)
             .buildParsed();
  }
  public static Baz parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    Builder builder = newBuilder();
    if (builder.mergeDelimitedFrom(input)) {
      return builder.buildParsed();
    } else {
      return null;
    }
  }
  public static Baz parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    Builder builder = newBuilder();
    if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
      return builder.buildParsed();
    } else {
      return null;
    }
  }
  public static Baz parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input).buildParsed();
  }
  public static Baz parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input, extensionRegistry)
             .buildParsed();
  }
  
  public static Builder newBuilder() { return Builder.create(); }
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder(Baz prototype) {
    return newBuilder().mergeFrom(prototype);
  }
  public Builder toBuilder() { return newBuilder(this); }
  
  @java.lang.Override
  protected Builder newBuilderForType(
      com.google.protobuf.GeneratedMessage.BuilderParent parent) {
    Builder builder = new Builder(parent);
    return builder;
  }
  public static final class Builder extends
      com.google.protobuf.GeneratedMessage.Builder<Builder>
     implements BazOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return HelloWorld.internal_static_Baz_descriptor;
    }
    
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return HelloWorld.internal_static_Baz_fieldAccessorTable;
    }
    
    // Construct using Baz.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }
    
    private Builder(BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
      }
    }
    private static Builder create() {
      return new Builder();
    }
    
    public Builder clear() {
      super.clear();
      return this;
    }
    
    public Builder clone() {
      return create().mergeFrom(buildPartial());
    }
    
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return Baz.getDescriptor();
    }
    
    public Baz getDefaultInstanceForType() {
      return Baz.getDefaultInstance();
    }
    
    public Baz build() {
      Baz result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }
    
    private Baz buildParsed()
        throws com.google.protobuf.InvalidProtocolBufferException {
      Baz result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(
          result).asInvalidProtocolBufferException();
      }
      return result;
    }
    
    public Baz buildPartial() {
      Baz result = new Baz(this);
      onBuilt();
      return result;
    }
    
    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof Baz) {
        return mergeFrom((Baz)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }
    
    public Builder mergeFrom(Baz other) {
      if (other == Baz.getDefaultInstance()) return this;
      this.mergeUnknownFields(other.getUnknownFields());
      return this;
    }
    
    public final boolean isInitialized() {
      return true;
    }
    
    public Builder mergeFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      com.google.protobuf.UnknownFieldSet.Builder unknownFields =
        com.google.protobuf.UnknownFieldSet.newBuilder(
          this.getUnknownFields());
      while (true) {
        int tag = input.readTag();
        switch (tag) {
          case 0:
            this.setUnknownFields(unknownFields.build());
            onChanged();
            return this;
          default: {
            if (!parseUnknownField(input, unknownFields,
                                   extensionRegistry, tag)) {
              this.setUnknownFields(unknownFields.build());
              onChanged();
              return this;
            }
            break;
          }
        }
      }
    }
    
    
    // @@protoc_insertion_point(builder_scope:Baz)
  }
  
  static {
    defaultInstance = new Baz(true);
    defaultInstance.initFields();
  }
  
  // @@protoc_insertion_point(class_scope:Baz)
  public static final int OPT_OTHER_EXT_FIELD_NUMBER = 101;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      Other> optOtherExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        0,
        Other.class,
        Other.getDefaultInstance());
  public static final int OPT_BAZ_EXT_FIELD_NUMBER = 102;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      Baz> optBazExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        1,
        Baz.class,
        Baz.getDefaultInstance());
  public static final int OPT_INT_EXT_FIELD_NUMBER = 103;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      java.lang.Integer> optIntExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        2,
        java.lang.Integer.class,
        null);
  public static final int OPT_FLAGS_EXT_FIELD_NUMBER = 104;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      Flags> optFlagsExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        3,
        Flags.class,
        null);
  public static final int OPT_STR_EXT_FIELD_NUMBER = 105;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      java.lang.String> optStrExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        4,
        java.lang.String.class,
        null);
  public static final int REP_OTHER_EXT_FIELD_NUMBER = 111;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      java.util.List<Other>> repOtherExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        5,
        Other.class,
        Other.getDefaultInstance());
  public static final int REP_BAZ_EXT_FIELD_NUMBER = 112;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      java.util.List<Baz>> repBazExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        6,
        Baz.class,
        Baz.getDefaultInstance());
  public static final int REP_INT_EXT_FIELD_NUMBER = 113;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      java.util.List<java.lang.Integer>> repIntExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        7,
        java.lang.Integer.class,
        null);
  public static final int REP_FLAGS_EXT_FIELD_NUMBER = 114;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      java.util.List<Flags>> repFlagsExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        8,
        Flags.class,
        null);
  public static final int REP_STR_EXT_FIELD_NUMBER = 115;
  public static final
    com.google.protobuf.GeneratedMessage.GeneratedExtension<
      Foo,
      java.util.List<java.lang.String>> repStrExt = com.google.protobuf.GeneratedMessage
          .newMessageScopedGeneratedExtension(
        Baz.getDefaultInstance(),
        9,
        java.lang.String.class,
        null);
}

