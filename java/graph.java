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

  public int GetVerticesNum() {
    return GetVerticesNum(ptr);
  }

  public IteratorPair InnerVertices() {
    return InnerVertices(ptr);
  }

  public IteratorPair GetOutgoingEdgesLid(long lid) {
    return GetOutgoingEdgesLid(ptr, lid);
  }

  public void SetPResult(long vertexPtr, final double r, boolean init) {
    SetPResult(ptr, vertexPtr, r, init);
  }

  public void SetPResult(long lid, final double r) {
    SetPResult(ptr, lid, r);
  }

  public double GetPResult(long lid) {
    return GetPResult(ptr, lid);
  }

  public long GetVertexByLid(long lid) {
    return GetVertexBylid(ptr, lid);
  }

  public double GetData (long eptr) {
    return GetData(ptr, eptr);
  }


  /** native method **/
  private native long GetVertexBylid (long ptr, long lid);

  private native double GetPResult(long ptr, long lid);

  private native double GetData(long ptr, long eptr);

  private native int GetVerticesNum(long ptr);

  private native IteratorPair InnerVertices(long ptr);

  private native IteratorPair GetOutgoingEdgesLid(long ptr, long lid);

  private native void SetPResult (long ptr, long vertexPtr, final double r, boolean init);

  private native void SetPResult (long ptr, long lid, final double r);

  private native void SetAutoPResult (long ptr, long vertexPtr, final double r, boolean init);

  private long ptr;

}
