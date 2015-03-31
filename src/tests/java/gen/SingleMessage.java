// Generated by the protocol buffer compiler.  DO NOT EDIT!

public  final class SingleMessage extends
    com.google.protobuf.GeneratedMessage
    implements SingleMessageOrBuilder {
  // Use SingleMessage.newBuilder() to construct.
  private SingleMessage(Builder builder) {
    super(builder);
  }
  private SingleMessage(boolean noInit) {}
  
  private static final SingleMessage defaultInstance;
  public static SingleMessage getDefaultInstance() {
    return defaultInstance;
  }
  
  public SingleMessage getDefaultInstanceForType() {
    return defaultInstance;
  }
  
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return HelloWorld.internal_static_SingleMessage_descriptor;
  }
  
  protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return HelloWorld.internal_static_SingleMessage_fieldAccessorTable;
  }
  
  private int bitField0_;
  // required .EmptyMessage req_msg = 1;
  public static final int REQ_MSG_FIELD_NUMBER = 1;
  private EmptyMessage reqMsg_;
  public boolean hasReqMsg() {
    return ((bitField0_ & 0x00000001) == 0x00000001);
  }
  public EmptyMessage getReqMsg() {
    return reqMsg_;
  }
  public EmptyMessageOrBuilder getReqMsgOrBuilder() {
    return reqMsg_;
  }
  
  private void initFields() {
    reqMsg_ = EmptyMessage.getDefaultInstance();
  }
  private byte memoizedIsInitialized = -1;
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized != -1) return isInitialized == 1;
    
    if (!hasReqMsg()) {
      memoizedIsInitialized = 0;
      return false;
    }
    memoizedIsInitialized = 1;
    return true;
  }
  
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    getSerializedSize();
    if (((bitField0_ & 0x00000001) == 0x00000001)) {
      output.writeMessage(1, reqMsg_);
    }
    getUnknownFields().writeTo(output);
  }
  
  private int memoizedSerializedSize = -1;
  public int getSerializedSize() {
    int size = memoizedSerializedSize;
    if (size != -1) return size;
  
    size = 0;
    if (((bitField0_ & 0x00000001) == 0x00000001)) {
      size += com.google.protobuf.CodedOutputStream
        .computeMessageSize(1, reqMsg_);
    }
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
  
  public static SingleMessage parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data).buildParsed();
  }
  public static SingleMessage parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data, extensionRegistry)
             .buildParsed();
  }
  public static SingleMessage parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data).buildParsed();
  }
  public static SingleMessage parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return newBuilder().mergeFrom(data, extensionRegistry)
             .buildParsed();
  }
  public static SingleMessage parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input).buildParsed();
  }
  public static SingleMessage parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input, extensionRegistry)
             .buildParsed();
  }
  public static SingleMessage parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    Builder builder = newBuilder();
    if (builder.mergeDelimitedFrom(input)) {
      return builder.buildParsed();
    } else {
      return null;
    }
  }
  public static SingleMessage parseDelimitedFrom(
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
  public static SingleMessage parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input).buildParsed();
  }
  public static SingleMessage parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return newBuilder().mergeFrom(input, extensionRegistry)
             .buildParsed();
  }
  
  public static Builder newBuilder() { return Builder.create(); }
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder(SingleMessage prototype) {
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
     implements SingleMessageOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return HelloWorld.internal_static_SingleMessage_descriptor;
    }
    
    protected com.google.protobuf.GeneratedMessage.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return HelloWorld.internal_static_SingleMessage_fieldAccessorTable;
    }
    
    // Construct using SingleMessage.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }
    
    private Builder(BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessage.alwaysUseFieldBuilders) {
        getReqMsgFieldBuilder();
      }
    }
    private static Builder create() {
      return new Builder();
    }
    
    public Builder clear() {
      super.clear();
      if (reqMsgBuilder_ == null) {
        reqMsg_ = EmptyMessage.getDefaultInstance();
      } else {
        reqMsgBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000001);
      return this;
    }
    
    public Builder clone() {
      return create().mergeFrom(buildPartial());
    }
    
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return SingleMessage.getDescriptor();
    }
    
    public SingleMessage getDefaultInstanceForType() {
      return SingleMessage.getDefaultInstance();
    }
    
    public SingleMessage build() {
      SingleMessage result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }
    
    private SingleMessage buildParsed()
        throws com.google.protobuf.InvalidProtocolBufferException {
      SingleMessage result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(
          result).asInvalidProtocolBufferException();
      }
      return result;
    }
    
    public SingleMessage buildPartial() {
      SingleMessage result = new SingleMessage(this);
      int from_bitField0_ = bitField0_;
      int to_bitField0_ = 0;
      if (((from_bitField0_ & 0x00000001) == 0x00000001)) {
        to_bitField0_ |= 0x00000001;
      }
      if (reqMsgBuilder_ == null) {
        result.reqMsg_ = reqMsg_;
      } else {
        result.reqMsg_ = reqMsgBuilder_.build();
      }
      result.bitField0_ = to_bitField0_;
      onBuilt();
      return result;
    }
    
    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof SingleMessage) {
        return mergeFrom((SingleMessage)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }
    
    public Builder mergeFrom(SingleMessage other) {
      if (other == SingleMessage.getDefaultInstance()) return this;
      if (other.hasReqMsg()) {
        mergeReqMsg(other.getReqMsg());
      }
      this.mergeUnknownFields(other.getUnknownFields());
      return this;
    }
    
    public final boolean isInitialized() {
      if (!hasReqMsg()) {
        
        return false;
      }
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
          case 10: {
            EmptyMessage.Builder subBuilder = EmptyMessage.newBuilder();
            if (hasReqMsg()) {
              subBuilder.mergeFrom(getReqMsg());
            }
            input.readMessage(subBuilder, extensionRegistry);
            setReqMsg(subBuilder.buildPartial());
            break;
          }
        }
      }
    }
    
    private int bitField0_;
    
    // required .EmptyMessage req_msg = 1;
    private EmptyMessage reqMsg_ = EmptyMessage.getDefaultInstance();
    private com.google.protobuf.SingleFieldBuilder<
        EmptyMessage, EmptyMessage.Builder, EmptyMessageOrBuilder> reqMsgBuilder_;
    public boolean hasReqMsg() {
      return ((bitField0_ & 0x00000001) == 0x00000001);
    }
    public EmptyMessage getReqMsg() {
      if (reqMsgBuilder_ == null) {
        return reqMsg_;
      } else {
        return reqMsgBuilder_.getMessage();
      }
    }
    public Builder setReqMsg(EmptyMessage value) {
      if (reqMsgBuilder_ == null) {
        if (value == null) {
          throw new NullPointerException();
        }
        reqMsg_ = value;
        onChanged();
      } else {
        reqMsgBuilder_.setMessage(value);
      }
      bitField0_ |= 0x00000001;
      return this;
    }
    public Builder setReqMsg(
        EmptyMessage.Builder builderForValue) {
      if (reqMsgBuilder_ == null) {
        reqMsg_ = builderForValue.build();
        onChanged();
      } else {
        reqMsgBuilder_.setMessage(builderForValue.build());
      }
      bitField0_ |= 0x00000001;
      return this;
    }
    public Builder mergeReqMsg(EmptyMessage value) {
      if (reqMsgBuilder_ == null) {
        if (((bitField0_ & 0x00000001) == 0x00000001) &&
            reqMsg_ != EmptyMessage.getDefaultInstance()) {
          reqMsg_ =
            EmptyMessage.newBuilder(reqMsg_).mergeFrom(value).buildPartial();
        } else {
          reqMsg_ = value;
        }
        onChanged();
      } else {
        reqMsgBuilder_.mergeFrom(value);
      }
      bitField0_ |= 0x00000001;
      return this;
    }
    public Builder clearReqMsg() {
      if (reqMsgBuilder_ == null) {
        reqMsg_ = EmptyMessage.getDefaultInstance();
        onChanged();
      } else {
        reqMsgBuilder_.clear();
      }
      bitField0_ = (bitField0_ & ~0x00000001);
      return this;
    }
    public EmptyMessage.Builder getReqMsgBuilder() {
      bitField0_ |= 0x00000001;
      onChanged();
      return getReqMsgFieldBuilder().getBuilder();
    }
    public EmptyMessageOrBuilder getReqMsgOrBuilder() {
      if (reqMsgBuilder_ != null) {
        return reqMsgBuilder_.getMessageOrBuilder();
      } else {
        return reqMsg_;
      }
    }
    private com.google.protobuf.SingleFieldBuilder<
        EmptyMessage, EmptyMessage.Builder, EmptyMessageOrBuilder> 
        getReqMsgFieldBuilder() {
      if (reqMsgBuilder_ == null) {
        reqMsgBuilder_ = new com.google.protobuf.SingleFieldBuilder<
            EmptyMessage, EmptyMessage.Builder, EmptyMessageOrBuilder>(
                reqMsg_,
                getParentForChildren(),
                isClean());
        reqMsg_ = null;
      }
      return reqMsgBuilder_;
    }
    
    // @@protoc_insertion_point(builder_scope:SingleMessage)
  }
  
  static {
    defaultInstance = new SingleMessage(true);
    defaultInstance.initFields();
  }
  
  // @@protoc_insertion_point(class_scope:SingleMessage)
}
