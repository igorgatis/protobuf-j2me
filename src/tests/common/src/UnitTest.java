import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public interface UnitTest {

  public Object newMessage() throws IOException;

  public void writeTo(Object msg, OutputStream stream) throws IOException;

  public Object readFrom(InputStream stream) throws IOException;
}
