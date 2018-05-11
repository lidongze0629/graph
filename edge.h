#ifndef GRAPE_FRAGMENT_EDGE_H
#define GRAPE_FRAGMENT_EDGE_H

namespace grape {
    class Edge {
    public:
        /** Default constructor **/
        Edge () {}

        Edge (int src, int dst, const double data):
            src_(src), dst_(dst), data_(data) {}

        inline const int src () const { return src_; }

        inline const int dst () const { return dst_; }

        inline const double &edata() const {return data();}

        void set_data (const double &data) { data_ = data; }

        void SetInfo (int src, int dst, const double &data) {
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

        int src_, dst_;
        double data_;
    };
}

#endif
