import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.google.protobuf.Message;

public abstract class JavaUnitTestAdapter implements UnitTest {

  public Object newMessage() throws IOException {
    return produce();
  }

  public void writeTo(Object msg, OutputStream stream) throws IOException {
    ((Message) msg).writeTo(stream);
  }

  public Object readFrom(InputStream stream) throws IOException {
    return parse(stream);
  }

  public abstract Message produce() throws IOException;

  public abstract Message parse(InputStream in) throws IOException;
}
