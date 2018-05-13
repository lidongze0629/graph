#ifndef GRAPE_FRAGMENT_VERTEX_H_
#define GRAPE_FRAGMENT_VERTEX_H_

namespace grape {
    class Vertex {
    public:
        Vertex () {}

        Vertex (int vid, double data):
                vid_(vid),data_(data) {}

        ~Vertex() {}

        inline const int vid () const { return vid_; }
        inline const double &vdata () const { return data(); }

        void set_vid (const int vid) { vid_ = vid; }
        void set_data (const double &data) { data_ = data; }

        void SetInfo (const int vid, const double &data) {
            vid_ = vid;
            data_ = data;
        }


    private:
        inline const double &data() const { return data_; }

        int vid_;
        double data_;
    };
}

#endif