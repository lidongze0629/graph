public class graph {

  static {
    System.loadLibrary("graph_core"); // myjni.dll (Windows) or libmyjni.so (Unixes)
  }

  /** Default constructor that does nothing. */
  graph() {}

  /** constructor that from grape fragment call. */
  graph(long ptr) {
    this.ptr = ptr;
  }

  /**
   *
   * @param ptr the (long type)pointer which correspond to the fragment addr
   */
  public void setPtr(long ptr) {
    this.ptr = ptr;
  }

  public long getPtr() { return ptr; }


  public native int GetVerticesNum(long ptr);

  public native IteratorPair InnerVertices(long ptr);

  public native void SetAutoPResult (long ptr, long vertexPtr, final double r, boolean init);

  private long ptr;

}
