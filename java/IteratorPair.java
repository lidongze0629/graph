import java.util.Iterator;

public class IteratorPair implements Iterator{
  static {
    System.loadLibrary("graph_core");
  }

  /** Default constructor that does nothing. */
  IteratorPair() {}

  /**
   *
   * @param begin   grape {Iterator_pair.h begin_} address
   * @param end     grape {Iterator_pair.h end_} address
   * @param sizeof  the sizeof of Iterator_pair.h template T
   */
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

  private long begin_, end_, current_, cursor_;
  private int sizeof_;

}