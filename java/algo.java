import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.PriorityQueue;
import javafx.util.Pair;

public class algo extends IApp{

  static {
    System.loadLibrary("graph_core"); // myjni.dll (Windows) or libmyjni.so (Unixes)
  }

  /** Default constructor that does nothing. */
  algo () {}

  algo (graph g) { super(g); }

  public static Comparator<Pair<Double, Long>> PairComparator = new Comparator<Pair<Double, Long>>(){

    @Override
    public int compare(Pair<Double, Long> o1, Pair<Double, Long> o2) {
      if (o1.getKey() == o2.getKey()) {
        return 0;
      } else {
        return o1.getKey() < o2.getKey() ? -1 : 1;
      }
    }
  };

  @Override
  public void Init(graph g) {
    System.out.println("/******* begin sssp init *********/");
    IteratorPair inner_vertices = g.InnerVertices();
    while (inner_vertices.hasNext()) {
      long vertexPtr = inner_vertices.next();
      g.SetPResult(vertexPtr, Double.MAX_VALUE, true);
    }
    System.out.println("/******* end sssp init *********/");
  }

  @Override
  public void PEval(graph g, long query) {
    System.out.println("/******* begin sssp PEval *********/");
    System.out.println("Query source = " + query);

    int tvnum = g.GetVerticesNum();

    PriorityQueue<Pair<Double, Long>> heap = new PriorityQueue<Pair<Double, Long>>(1000,PairComparator);
    Boolean[] visited = new Boolean[tvnum];
    Arrays.fill(visited, false);

    long source_lid = query;
    double source_distance = 0.0;
    g.SetPResult(source_lid, source_distance);
    heap.add(new Pair<Double, Long>(0.0, source_lid));

    while (!heap.isEmpty()) {
      Long utemp = heap.peek().getValue();
      int u = utemp.intValue();
      heap.poll();

      if (visited[u]) continue;
      visited[u] = true;

      IteratorPair es = g.GetOutgoingEdgesLid(utemp);
      while (es.hasNext()) {
        long edgePtr = es.next();
        long v = Edge.dst(edgePtr);
        double len = g.GetData(edgePtr);

        double distv = g.GetPResult(v);
        double distu = g.GetPResult(u);

        if (distv > distu + len) {
          distv = distu + len;
          g.SetPResult(v, distv);
          heap.add(new Pair<Double, Long>(distv, v));
        }
      }
    }
    System.out.println("/******* end sssp PEval *********/");
  }

  @Override
  public void WriteToFile(graph g, String prefix) {

    int tvnum = g.GetVerticesNum();

    PrintWriter writer;

    try {
      writer = new PrintWriter(prefix, "UTF-8");
      for (int i = 0; i < tvnum; i++) {
        double result = g.GetPResult(i);
        writer.println(i + "\t" + (int)result);
      }
      writer.flush();
      writer.close();
    } catch (FileNotFoundException e) {
      e.printStackTrace();
    } catch (UnsupportedEncodingException e) {
      e.printStackTrace();
    }
  }

  public static void main (String[] args) {


  }
}
