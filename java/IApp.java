public abstract class IApp {

  static {
    System.loadLibrary("graph_core"); // myjni.dll (Windows) or libmyjni.so (Unixes)
  }

  /** Default constructor that does nothing. */
  protected IApp() {}

  protected IApp(graph g) { this.g = g; }

  public abstract void Init (graph g);

  public abstract void PEval (graph g, int query);


  private graph g;

}
