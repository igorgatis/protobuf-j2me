import java.io.IOException;
import java.io.InputStream;

import com.google.protobuf.ByteString;
import com.google.protobuf.Message;

public class HelloWorldTests implements TestSet {

  public void registerTests(TestDriver driver) {
    driver.register(new PrimitiveTypesOnlyTest());
    driver.register(new EmptyMessageTest());
    driver.register(new SingleMessageTest());
    driver.register(new QuickExtensionTest());
    driver.register(new HelloWorldProtoTest());
  }

  public static class PrimitiveTypesOnlyTest extends JavaUnitTestAdapter {
    public Message produce() throws IOException {
      PrimitiveTypesOnly.Builder msg = PrimitiveTypesOnly.newBuilder();
      msg.setReqBool(true);
      msg.setReqInt(1);
      msg.setReqStr("Hello");
      return msg.build();
    }

    public Message parse(InputStream input) throws IOException {
      PrimitiveTypesOnly.Builder msg = PrimitiveTypesOnly.newBuilder();
      msg.mergeFrom(input);
      return msg.build();
    }
  }

  public static class EmptyMessageTest extends JavaUnitTestAdapter {
    public Message produce() throws IOException {
      EmptyMessage.Builder msg = EmptyMessage.newBuilder();
      return msg.build();
    }

    public Message parse(InputStream input) throws IOException {
      EmptyMessage.Builder msg = EmptyMessage.newBuilder();
      msg.mergeFrom(input);
      return msg.build();
    }
  }

  public static class SingleMessageTest extends JavaUnitTestAdapter {
    public Message produce() throws IOException {
      SingleMessage.Builder msg = SingleMessage.newBuilder();
      msg.setReqMsg(EmptyMessage.newBuilder().build());
      return msg.build();
    }

    public Message parse(InputStream input) throws IOException {
      SingleMessage.Builder msg = SingleMessage.newBuilder();
      msg.mergeFrom(input);
      return msg.build();
    }
  }

  public static class QuickExtensionTest extends JavaUnitTestAdapter {
    public Message produce() throws IOException {
      Foo.Builder foo = Foo.newBuilder();
      foo.setOptStr("opt_str");
      foo.setExtension(HelloWorld.optOtherExt, Other.newBuilder().build());
      foo.setExtension(Baz.optOtherExt, Other.newBuilder().build());
      foo.setExtension(Baz.optBazExt, Baz.newBuilder().build());
      foo.setExtension(Baz.optIntExt, new Integer(10));
      foo.setExtension(Baz.optFlagsExt, Flags.VAL2);
      foo.setExtension(Baz.optStrExt, "optStrExt");

      foo.addExtension(Baz.repOtherExt, Other.newBuilder().build());
      foo.addExtension(Baz.repBazExt, Baz.newBuilder().build());
      foo.addExtension(Baz.repIntExt, new Integer(10));
      foo.addExtension(Baz.repFlagsExt, Flags.VAL2);
      foo.addExtension(Baz.repStrExt, "repStrExt");

      foo.addExtension(Baz.repOtherExt, Other.newBuilder().build());
      foo.addExtension(Baz.repBazExt, Baz.newBuilder().build());
      foo.addExtension(Baz.repIntExt, new Integer(10));
      foo.addExtension(Baz.repFlagsExt, Flags.VAL2);
      foo.addExtension(Baz.repStrExt, "repStrExt");
      return foo.build();
    }

    public Message parse(InputStream input) throws IOException {
      return Foo.parseFrom(input);
    }
  }

  public static class HelloWorldProtoTest extends JavaUnitTestAdapter {
    public Message produce() throws IOException {
      HelloWorldProto.Builder msg = HelloWorldProto.newBuilder();
      msg.setReqBool(true);
      msg.setReqBoolDef(true);
      msg.setReqBts(ByteString.copyFromUtf8("Hello World"));
      msg.setReqFlg(Flags.VAL2);
      msg.setReqFlgDef(Flags.VAL2);
      msg.setReqFoo(Foo.newBuilder().build());
      msg.setReqInt(42);
      msg.setReqIntDef(33);
      msg.setReqStr("Hi there");
      msg.setReqStrDef("default string");
      return msg.build();
    }

    public Message parse(InputStream input) throws IOException {
      return HelloWorldProto.parseFrom(input);
    }
  }
}
