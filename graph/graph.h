#ifndef GRAPE_FRAGMENT_GRAPH_H_
#define GRAPE_FRAGMENT_GRAPH_H_

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <jni.h>

#include "edge.h"
#include "vertex.h"
#include "Iterator_pair.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     graph
 * Method:    GetVerticesNum
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_graph_GetVerticesNum
        (JNIEnv *, jobject, jlong);

/*
 * Class:     graph
 * Method:    InnerVertices
 * Signature: (J)LIteratorPair;
 */
JNIEXPORT jobject JNICALL Java_graph_InnerVertices
        (JNIEnv *, jobject, jlong);

/*
 * Class:     graph
 * Method:    SetAutoPResult
 * Signature: (JJDZ)V
 */
JNIEXPORT void JNICALL Java_graph_SetAutoPResult
(JNIEnv *, jobject, jlong, jlong, jdouble, jboolean);

#ifdef __cplusplus
}
#endif

namespace grape {
    class Graph {

    private:
        /** An iterator impl for {@code edge_iterator} .*/
        class IMEItImpl {
        public:
            explicit IMEItImpl(std::vector<Edge>::iterator it) : iter(it) {}

            IMEItImpl *clone() { return new IMEItImpl(iter); }

            void increment() { iter++; }

            void add (int right) { iter += right; }

            Edge &deref() { return *iter; }

            Edge *pointer() { return &(*iter); }
        private:
            std::vector<Edge>::iterator iter;
        };

    public:
        Graph () {}

        ~Graph () {}

        /** An iterator of Vertex . */
        typedef std::vector<Vertex>::iterator vertex_iterator;

        /** An iterator of Edge . */
        class edge_iterator {
        public:
            edge_iterator () : impl() {}

            explicit edge_iterator(IMEItImpl *impl) : impl(impl) {}

            edge_iterator (edge_iterator const &right) : impl(right.impl->clone()) {}

            ~edge_iterator() { delete impl; }

            edge_iterator &operator=(edge_iterator const &right) {
                delete impl;
                impl = right.impl->clone();
                return *this;
            }

            edge_iterator &operator++() {
                impl->increment();
                return *this;
            }

            edge_iterator operator++(int) {
                edge_iterator it(*this);
                impl->increment();
                return it;
            }

            Edge &operator*() { return impl->deref(); }

            Edge *operator->() { return impl->pointer(); }

            bool operator==(const edge_iterator &it) {
                return (impl->pointer() == it.impl->pointer());
            }

            bool operator!=(const edge_iterator &it) {
                return !(impl->pointer() == it.impl->pointer());
            }

        private:
            IMEItImpl *impl;
        };

        inline int GetVerticesNum() const { return vlist_.size(); }

        IteratorPair<vertex_iterator> InnerVertices() {
             return IteratorPair<vertex_iterator >(vlist_.begin(), vlist_.end());
        }

        inline const double &GetVertexData (unsigned vid) {
            return vlist_[vid].vdata();
        }

        inline void set_presult_on_vertex (std::vector<double> *presult) {
            presult_on_vertex_ = presult;
        }

        inline void SetPResult (const Vertex &v, const double &r, bool init) {
            presult_on_vertex_->at(v.vid()) = r;
        }

        inline void SetPResult (const Vertex &v, const double &r) {
            presult_on_vertex_->at(v.vid()) = r;
        }

        inline void SetPResult (const unsigned lid, const double &r) {
            presult_on_vertex_->at(lid) = r;
        }

        inline const double &GetPResult (const Vertex &v) {
            return presult_on_vertex_->at(v.vid());
        }

        inline const double &GetPResult (const unsigned lid) {
            return presult_on_vertex_->at(lid);
        }

        inline Vertex &GetVertexByLid (const unsigned lid) { return vlist_[lid]; }

        inline Vertex &GetSrcVertex (const Edge &edge) { return vlist_[edge.src()]; }

        inline Vertex &GetDstVertex (const Edge &edge) { return vlist_[edge.dst()]; }

        inline const double &GetData (const Edge &e) { return e.edata(); }

        inline IteratorPair<edge_iterator> GetOutgoingEdgesLid (const unsigned lvid) {
            return IteratorPair<edge_iterator>(
                    edge_iterator(new IMEItImpl(oe_.begin() + oeoffset_[lvid])),
                    edge_iterator(new IMEItImpl(oe_.begin() + oeoffset_[lvid + 1])));
        };

        void LoadFromFile (std::vector<Edge> &ecut_edge, std::vector<Vertex> &vertices,
                           const std::string &vfile, const std::string &efile) {
            Loadefile(efile, ecut_edge);
            Loadvfile(vfile, vertices);
        }

        void Loadvfile (const std::string &vfile, std::vector<Vertex> &vertices) {
            std::cout << "Load vile \"" << vfile << "\""<< std::endl;
            std::string location = vfile;
            FILE *fin = fopen(location.c_str(), "r");
            if (!fin) { std::cout << "file " << location << " open failed!" << std::endl; }

            unsigned vid;
            double data;
            while (fscanf(fin, "%u%lg", &vid, &data) != EOF) {
                vertices.emplace_back(vid,data);
            }
            fclose(fin);
        }

        void Loadefile (const std::string &efile, std::vector<Edge> &ecut_edge) {
            std::cout << "Load eile \"" << efile << "\""<< std::endl;
            std::string location = efile;
            FILE *fin = fopen(location.c_str(), "r");
            if (!fin) { std::cout << "file " << location << " open failed!" << std::endl; }

            unsigned src, dst;
            double data;
            while (fscanf(fin, "%u%u%lg", &src, &dst, &data) != EOF) {
                ecut_edge.emplace_back(src, dst, data);
            }
            fclose(fin);
        }

        void InitGraph (std::vector<Vertex> &vertices, std::vector<Edge> &ecut_edges) {
            initVertices(vertices);
            initEdges(ecut_edges);
        }

    private:
        void initVertices (std::vector<Vertex> &vertices) {
            tvnum_ = vertices.size();
            vlist_.resize(tvnum_);
            for (auto &v : vertices) {
                unsigned vid = v.vid();
                vlist_[vid].SetInfo(vid, v.vdata());
            }
        }

        void initEdges (std::vector<Edge> &edges) {
            std::vector<int> oenum(tvnum_, 0);
            std::vector<int> ienum(tvnum_, 0);

            for (auto &e : edges) {
                unsigned u = e.src();
                unsigned v = e.dst();
                oenum[u]++;
                ienum[v]++;
            }

            oe_.resize(edges.size());
            ie_.resize(edges.size());

            ieoffset_.resize(tvnum_ + 1);
            oeoffset_.resize(tvnum_ + 1);

            ieoffset_[0] = 0;
            oeoffset_[0] = 0;

            for (unsigned i = 0; i < tvnum_; i++) {
                ieoffset_[i + 1] = ieoffset_[i] + ienum[i];
                oeoffset_[i + 1] = oeoffset_[i] + oenum[i];
            }

            std::vector<std::vector<Edge>::iterator> ieiter(tvnum_), oeiter(tvnum_);

            for (unsigned i = 0; i < tvnum_; i++) {
                ieiter[i] = ieoffset_[i] + ie_.begin();
                oeiter[i] = oeoffset_[i] + oe_.begin();
            }

            for (auto &e : edges) {
                unsigned u = e.src();
                unsigned v = e.dst();

                oeiter[u]->SetInfo(u, v, e.edata());
                ieiter[v]->SetInfo(u, v, e.edata());

                oeiter[u]++;
                ieiter[v]++;
            }

            for (unsigned i = 0; i < tvnum_; i++) {
                std::sort(
                    ie_.begin() + ieoffset_[i], ie_.begin() + ieoffset_[i + 1],
                    [](const Edge &e1, const Edge &e2) { return e1.src() < e2.src();});
                std::sort(
                    oe_.begin() + oeoffset_[i], oe_.begin() + oeoffset_[i + 1],
                    [](const Edge &e1, const Edge &e2) { return e1.dst() < e2.dst(); });
            }
        }

        unsigned ivnum_, ovnum_, tvnum_;
        std::vector<Vertex> vlist_;

        std::vector<Edge> ie_, oe_;
        std::vector<int> ieoffset_, oeoffset_;

        std::vector<double> *presult_on_vertex_;
    };
}

#endif