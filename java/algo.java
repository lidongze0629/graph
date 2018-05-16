public class algo extends IApp{

  static {
    System.loadLibrary("graph_core"); // myjni.dll (Windows) or libmyjni.so (Unixes)
  }

  /** Default constructor that does nothing. */
  algo () {}

  algo (graph g) { super(g); }

  @Override
  public void Init(graph g) {
    System.out.println("java init.");
    System.out.println(g.getPtr());
  }

  @Override
  public void PEval(graph g, int query) {
    System.out.println("java PEval.");
  }


  public static void main (String[] args) {
    graph g = new graph(10);
    algo a = new algo(g);
    a.Init(g);
  }
}
