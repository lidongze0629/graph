#ifndef GRAPE_FRAGMENT_EDGE_H
#define GRAPE_FRAGMENT_EDGE_H

#include <iostream>

namespace grape {
    class Edge {
    public:
        /** Default constructor **/
        Edge () {}

        Edge (unsigned src, unsigned dst, const double data):
            src_(src), dst_(dst), data_(data) {}

        inline const unsigned src () const { return src_; }

        inline const unsigned dst () const { return dst_; }

        inline const double &edata() const {return data();}

        void set_data (const double &data) { data_ = data; }

        void SetInfo (unsigned src, unsigned dst, const double &data) {
            src_ = src;
            dst_ = dst;
            data_ = data;
        }

        Edge &operator=(const Edge &edge) {
            this->src_ = edge.src();
            this->dst_ = edge.dst();
            this->data_ = edge.data();
            return *this;
        }

    private:

        inline const double &data() const { return data_;}

        unsigned src_, dst_;
        double data_;

        friend std::ostream &operator<< (std::ostream &out, const Edge &e);
        friend std::istream &operator>> (std::istream &in, const Edge &e);
    };

inline std::ostream &operator<< (std::ostream &out, const Edge &e) {
    out << e.src() << "->" << e.dst() << "(" << e.edata() << ")";
    return out;
}
}

#endif
