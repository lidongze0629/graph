#ifndef GRAPE_FRAGMENT_VERTEX_H_
#define GRAPE_FRAGMENT_VERTEX_H_

#include <iostream>

namespace grape {
    class Vertex {
    public:
        Vertex () {}

        Vertex (unsigned vid, double data):
                vid_(vid),data_(data) {}

        ~Vertex() {}

        inline const unsigned vid () const { return vid_; }
        inline const double &vdata () const { return data(); }

        void set_vid (const unsigned vid) { vid_ = vid; }
        void set_data (const double &data) { data_ = data; }

        void SetInfo (const unsigned vid, const double &data) {
            vid_ = vid;
            data_ = data;
        }
        
    private:
        inline const double &data() const { return data_; }

        unsigned vid_;
        double data_;

        friend std::ostream &operator<< (std::ostream &out, const Vertex &v);
        friend std::istream &operator>> (std::istream &in, const Vertex &v);
    };

inline std::ostream &operator<< (std::ostream &out, const Vertex &v) {
    out << v.vid() << "(" << v.vdata() << ")";
    return out;
}
}

#endif