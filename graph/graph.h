#ifndef GRAPE_FRAGMENT_GRAPH_H_
#define GRAPE_FRAGMENT_GRAPH_H_

#include <vector>
#include <string>
#include <fstream>
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

        inline int GetVerticesNum() const { return vertices_.size(); }

        IteratorPair<std::vector<Vertex>::iterator> InnerVertices() {
             return IteratorPair<std::vector<Vertex>::iterator>(vertices_.begin(), vertices_.end());
        }

        inline const double &GetVertexData (int vid) {
            return vertices_[vid].vdata();
        }

        inline std::vector<Edge> &GetOutgoingEdges (int vid) { return oe_[vid]; }
        inline std::vector<Edge> &GetIncomingEdges (int vid) { return ie_[vid]; }

        inline bool HasParent (int vid) { return !ie_[vid].empty(); }
        inline bool HasChild (int vid) { return !oe_[vid].empty(); }

        void LoadFromFile (const std::string &vfile, const std::string &efile) {
            Loadvfile(vfile);
            Loadefile(efile);
        }

        void Loadvfile (const std::string &vfile) {
            std::cout << "Load vile \"" << vfile << "\""<< std::endl;
            std::string location = vfile;
            FILE *fin = fopen(location.c_str(), "r");
            if (!fin) { std::cout << "file " << location << " open failed!" << std::endl; }

            int vid, vnum = 0;
            double data;
            while (fscanf(fin, "%d%lg", &vid, &data) != EOF) {
                vnum++;
                vertices_.emplace_back(vid,data);
            }
            ie_.resize(vnum), oe_.resize(vnum);
            fclose(fin);
        }

        void Loadefile (const std::string &efile) {
            std::cout << "Load eile \"" << efile << "\""<< std::endl;
            std::string location = efile;
            FILE *fin = fopen(location.c_str(), "r");
            if (!fin) { std::cout << "file " << location << " open failed!" << std::endl; }

            int src, dst;
            double data;
            while (fscanf(fin, "%d%d%lg", &src, &dst, &data) != EOF) {
                oe_[src].emplace_back(src, dst, data);
                ie_[dst].emplace_back(src, dst, data);
            }
        }

    private:
        std::vector<Vertex> vertices_;
        std::vector<std::vector<Edge>> ie_, oe_;
    };
}

#endif