#ifndef GRAPE_ITERATOR_PAIR_H_
#define GRAPE_ITERATOR_PAIR_H_

namespace grape {

template <typename T>

class IteratorPair {
public:
    IteratorPair(const T begin, const T end) : begin_(begin), end_(end) {}

    inline T begin() const { return begin_; }

    inline T end() const { return end_; }

    IteratorPair &operator=(const IteratorPair &ip) {
        this->begin_ = ip.begin();
        this->end_ = ip.end();
        return *this;
    }

private:
    T begin_, end_;
};
}
#endif