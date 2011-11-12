public class Main extends TestDriver {

  public static void main(String[] args) {
    Main main = new Main();
    main.parseParameters(args);
    new HelloWorldTests().registerTests(main);
    main.run();
  }

  @Override
  protected String getPlatformName() {
    return "java";
  }
}
