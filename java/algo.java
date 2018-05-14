import java.util.Iterator;

class IteratorPair implements Iterator{
  static {
    System.loadLibrary("graph_core");
  }

  private long begin_, end_, current_, cursor_;
  private int sizeof_;

  IteratorPair() {}

  IteratorPair(long begin, long end, int sizeof) {
    this.begin_ = begin;
    this.current_ = -1;     // index of last element returned;
    this.cursor_ = begin;   // index of the element to return;
    this.end_ = end;
    this.sizeof_ = sizeof;
  }

  @Override
  public boolean hasNext() {
    return cursor_ != end_;
  }

  @Override
  public Long next() {
    Long i = cursor_;
    cursor_  = i + sizeof_;
    return i;
  }

  @Override
  public void remove() {

  }

  @Override
  public String toString () {
    return String.format(begin_ + "->" + end_ + "(" + sizeof_ + ")");
  }
}

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

  public native IteratorPair InnerVertices(long ptr);

  public native void SetAutoPResult (long ptr, long vertexPtr, final double r, boolean init);

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
      // for int
      //int vnum = g.GetVerticesNum(g.getPtr());
      //System.out.println("java vnum: " + vnum);

      // for iterator
      IteratorPair inner_vertices = g.InnerVertices(g.getPtr());
      System.out.println("java iteratorPair: " + inner_vertices.toString());
      while (inner_vertices.hasNext()) {
        long vertexPtr = inner_vertices.next();
        System.out.println("java (long)vertexPtr: " + vertexPtr);
        g.SetAutoPResult(g.getPtr(), vertexPtr, 100, true);
      }
    }
  }

  public static void main (String[] args) {
    algo a = new algo(new graph(11));
    a.getPtr();
  }
}
