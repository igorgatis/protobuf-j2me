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

  public static class PrimitiveTypesOnlyTest extends J2meUnitTestAdapter {
    public Message produce() throws IOException {
      PrimitiveTypesOnly msg = new PrimitiveTypesOnly();
      msg.setReqBool(true);
      msg.setReqInt(1);
      msg.setReqStr("Hello");
      return msg;
    }

    public Message parse(InputStream input) throws IOException {
      return PrimitiveTypesOnly.parseFrom(input);
    }
  }

  public static class EmptyMessageTest extends J2meUnitTestAdapter {
    public Message produce() throws IOException {
      EmptyMessage msg = new EmptyMessage();
      return msg;
    }

    public Message parse(InputStream input) throws IOException {
      return EmptyMessage.parseFrom(input);
    }
  }

  public static class SingleMessageTest extends J2meUnitTestAdapter {
    public Message produce() throws IOException {
      SingleMessage msg = new SingleMessage();
      msg.setReqMsg(new EmptyMessage());
      return msg;
    }

    public Message parse(InputStream input) throws IOException {
      return SingleMessage.parseFrom(input);
    }
  }

  public static class QuickExtensionTest extends J2meUnitTestAdapter {
    public Message produce() throws IOException {
      Foo foo = new Foo();
      foo.setOptStr("opt_str");
      foo.setExtension(HelloWorld.optOtherExt, new Other());
      foo.setExtension(Baz.optOtherExt, new Other());
      foo.setExtension(Baz.optBazExt, new Baz());
      foo.setExtension(Baz.optIntExt, new Integer(10));
      foo.setExtension(Baz.optFlagsExt, Flags.VAL2);
      foo.setExtension(Baz.optStrExt, "optStrExt");

      foo.addExtension(Baz.repOtherExt, new Other());
      foo.addExtension(Baz.repBazExt, new Baz());
      foo.addExtension(Baz.repIntExt, new Integer(10));
      foo.addExtension(Baz.repFlagsExt, Flags.VAL2);
      foo.addExtension(Baz.repStrExt, "repStrExt");

      foo.addExtension(Baz.repOtherExt, new Other());
      foo.addExtension(Baz.repBazExt, new Baz());
      foo.addExtension(Baz.repIntExt, new Integer(10));
      foo.addExtension(Baz.repFlagsExt, Flags.VAL2);
      foo.addExtension(Baz.repStrExt, "repStrExt");
      return foo;
    }

    public Message parse(InputStream input) throws IOException {
      return Foo.parseFrom(input);
    }
  }

  public static class HelloWorldProtoTest extends J2meUnitTestAdapter {
    public Message produce() throws IOException {
      HelloWorldProto msg = new HelloWorldProto();
      msg.setReqBool(true);
      msg.setReqBoolDef(true);
      msg.setReqBts(ByteString.copyFromUtf8("Hello World"));
      msg.setReqFlg(Flags.VAL2);
      msg.setReqFlgDef(Flags.VAL2);
      msg.setReqFoo(new Foo());
      msg.setReqInt(42);
      msg.setReqIntDef(33);
      msg.setReqStr("Hi there");
      msg.setReqStrDef("default string");
      return msg;
    }

    public Message parse(InputStream input) throws IOException {
      return HelloWorldProto.parseFrom(input);
    }
  }
}
