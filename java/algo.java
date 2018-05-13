class graph {

  static {
    System.loadLibrary("graph_core"); // myjni.dll (Windows) or libmyjni.so (Unixes)
  }

  private long ptr;

  graph() {}

  graph(long p) {
    this.ptr = p;
  }

  public void setPtr(long ptr) {
    this.ptr = ptr;
  }

  public long getPtr() { return ptr; }

  public native int GetVerticesNum(long ptr);

}

public class algo {

  static {
    System.loadLibrary("graph_core"); // myjni.dll (Windows) or libmyjni.so (Unixes)
  }

  private graph g = null;

  algo () {}

  algo (graph g) { this.g = g; }

  public void getPtr() {
    if (g == null) {
      System.out.println("null");
    } else {
      System.out.println(g.GetVerticesNum(g.getPtr()));
    }
  }

  public static void main (String[] args) {
    algo a = new algo(new graph(11));
    a.getPtr();
  }
}
