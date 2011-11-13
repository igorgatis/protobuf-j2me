import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.security.InvalidParameterException;
import java.util.Vector;

public abstract class TestDriver {

  private boolean produce;
  private File path;
  private Vector<UnitTest> tests = new Vector<UnitTest>();

  public void register(UnitTest test) {
    tests.addElement(test);
  }

  protected abstract String getPlatformName();

  protected static class FakeOutputStream extends OutputStream {
    @Override
    public void write(int ch) throws IOException {
      char[] H = "0123456789ABCDEF".toCharArray();
      System.out.print(H[(0xFF & ch) / 16]);
      System.out.print(H[(0xFF & ch) % 16]);
      System.out.print(' ');
    }
  }

  private String getTestName(UnitTest test) {
    return test.getClass().getName().replace('$', '_');
  }

  private void produce(UnitTest test) throws IOException {
    File file = new File(path, getTestName(test) + ".bin");
    FileOutputStream stream = new FileOutputStream(file);
    Object msg = test.newMessage();
    test.writeTo(msg, stream);
    //test.writeTo(msg, new FakeOutputStream());
    stream.flush();
    stream.close();
  }

  private byte[] readAll(File file) throws IOException {
    FileInputStream stream = new FileInputStream(file);
    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    int read;
    while ((read = stream.read()) >= 0) {
      baos.write(read);
    }
    stream.close();
    return baos.toByteArray();
  }

  private boolean compare(byte[] a, byte[] b, StringBuffer errors) {
    if (a.length != b.length) {
      errors.append("  " + a.length + " != " + b.length);
      return false;
    }
    for (int i = 0; i < 0; i++) {
      if (a[i] != b[i]) {
        errors.append("  [" + i + "] " + a[i] + " != " + b[i]);
        return false;
      }
    }
    return true;
  }

  private boolean consume(UnitTest test, StringBuffer errors)
      throws IOException {
    Object origMsg = test.newMessage();
    File file = new File(path, getTestName(test) + ".bin");
    byte[] origData = readAll(file);
    Object msg = test.readFrom(new ByteArrayInputStream(origData));
    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    test.writeTo(msg, baos);
    byte[] data = baos.toByteArray();
    if (!origMsg.equals(msg)) {
      errors.append("  Messages are different.");
      return false;
    }
    return compare(origData, data, errors);
  }

  public void parseParameters(String[] args) {
    Boolean produce = null;
    for (int i = 0; i < args.length; i++) {
      if (args[i].equals("--produce") && produce == null) {
        produce = new Boolean(true);
        continue;
      }
      if (args[i].equals("--consume") && produce == null) {
        produce = new Boolean(false);
        continue;
      }
      if (args[i].equals("--path") && path == null && i + 1 < args.length) {
        i++;
        path = new File(args[i]);
        continue;
      }
      throw new InvalidParameterException(args[i]);
    }
    if (path == null) {
      System.err.println("Must specify --path parameter.");
      System.exit(1);
    }
    this.produce = produce == null || produce.booleanValue();
    if (this.produce && path.exists() == false) {
      path.mkdirs();
    }
  }

  public void run() {
    if (produce) {
      System.err.println(getPlatformName() + " produces: " + path);
      for (int i = 0; i < tests.size(); i++) {
        UnitTest test = (UnitTest) tests.elementAt(i);
        try {
          produce(test);
        } catch (IOException e) {
          // TODO Auto-generated catch block
          e.printStackTrace();
        }
      }
    } else {
      System.err.println(getPlatformName() + " consumes: " + path);
      for (int i = 0; i < tests.size(); i++) {
        UnitTest test = (UnitTest) tests.elementAt(i);
        boolean pass = false;
        Throwable exc = null;
        StringBuffer errors = new StringBuffer();
        try {
          pass = consume(test, errors);
        } catch (Throwable e) {
          exc = e;
        }
        if (pass) {
          System.err.println("  PASS " + getTestName(test));
        } else {
          System.err.println("  FAIL " + getTestName(test));
          if (errors.length() > 0) {
            System.err.println("  " + errors);
          }
          if (exc != null) {
            System.err.print("    ");
            exc.printStackTrace();
          }
        }
      }
      System.err.println();
    }
  }
}
