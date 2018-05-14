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
    public:
        Graph () {}

        ~Graph () {}

        inline int GetVerticesNum() const { return vlist_.size(); }

        IteratorPair<std::vector<Vertex>::iterator> InnerVertices() {
             return IteratorPair<std::vector<Vertex>::iterator>(vlist_.begin(), vlist_.end());
        }

        inline const double &GetVertexData (int vid) {
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

        inline void SetPResult (const int lid, const double &r) {
            presult_on_vertex_->at(lid) = r;
        }

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

            int vid;
            double data;
            while (fscanf(fin, "%d%lg", &vid, &data) != EOF) {
                vertices.emplace_back(vid,data);
            }
            fclose(fin);
        }

        void Loadefile (const std::string &efile, std::vector<Edge> &ecut_edge) {
            std::cout << "Load eile \"" << efile << "\""<< std::endl;
            std::string location = efile;
            FILE *fin = fopen(location.c_str(), "r");
            if (!fin) { std::cout << "file " << location << " open failed!" << std::endl; }

            int src, dst;
            double data;
            while (fscanf(fin, "%d%d%lg", &src, &dst, &data) != EOF) {
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
                int vid = v.vid();
                vlist_[vid].SetInfo(vid, v.vdata());
            }
        }

        void initEdges (std::vector<Edge> &edges) {
            std::vector<int> oenum(tvnum_, 0);
            std::vector<int> ienum(tvnum_, 0);

            for (auto &e : edges) {
                int u = e.src();
                int v = e.dst();
                oenum[u]++;
                ienum[v]++;
            }

            oe_.resize(edges.size());
            ie_.resize(edges.size());

            ieoffset_.resize(tvnum_ + 1);
            oeoffset_.resize(tvnum_ + 1);

            ieoffset_[0] = 0;
            oeoffset_[0] = 0;

            for (int i = 0; i < tvnum_; i++) {
                ieoffset_[i + 1] = ieoffset_[i] + ienum[i];
                oeoffset_[i + 1] = oeoffset_[i] + oenum[i];
            }

            std::vector<std::vector<Edge>::iterator> ieiter(tvnum_), oeiter(tvnum_);

            for (int i = 0; i < tvnum_; i++) {
                ieiter[i] = ieoffset_[i] + ie_.begin();
                oeiter[i] = oeoffset_[i] + oe_.begin();
            }

            for (auto &e : edges) {
                int u = e.src();
                int v = e.dst();

                oeiter[u]->SetInfo(u, v, e.edata());
                ieiter[v]->SetInfo(u, v, e.edata());

                oeiter[u]++;
                ieiter[v]++;
            }

            for (int i = 0; i < tvnum_; i++) {
                std::sort(
                    ie_.begin() + ieoffset_[i], ie_.begin() + ieoffset_[i + 1],
                    [](const Edge &e1, const Edge &e2) { return e1.src() < e2.src();});
                std::sort(
                    oe_.begin() + oeoffset_[i], oe_.begin() + oeoffset_[i + 1],
                    [](const Edge &e1, const Edge &e2) { return e1.dst() < e2.dst(); });
            }
        }

        int ivnum_, ovnum_, tvnum_;
        std::vector<Vertex> vlist_;

        std::vector<Edge> ie_, oe_;
        std::vector<int> ieoffset_, oeoffset_;

        std::vector<double> *presult_on_vertex_;
    };
}

#endif