#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <limits>
#include <fstream>

#include <jni.h>

#include <queue>
#include <unordered_set>

#include "graph/edge.h"
#include "graph/vertex.h"
#include "graph/graph.h"
#include "graph/Iterator_pair.h"

using grape::Edge;
using grape::Vertex;
using grape::Graph;
using grape::IteratorPair;

int main(int argc, char **argv) {

    if (argc != 3) { std::cout << "./graph-engine <query> <path_to_output_file>" << std::endl; exit(0); }

    std::unique_ptr<Graph> g(new Graph);

    /** load data, init graph */
    std::vector<Edge> edges;
    std::vector<Vertex> vertices;
    g->LoadFromFile (edges, vertices, "../data/twitter.v", "../data/twitter.e");
    g->InitGraph (vertices, edges);
    jlong address = (jlong)(uintptr_t)g.get();

    /** Init jvm environment. */
    JavaVM *jvm = nullptr;
    JNIEnv *env = nullptr;

    JavaVMOption options[1];
    JavaVMInitArgs vm_args;

    options[0].optionString = const_cast<char *>("-Djava.class.path=../java/");
    memset(&vm_args, 0, sizeof(vm_args));
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;

    int status = 1;
    status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

    /** pass graph(fragment) pointer as long type. */
    jobject graphObj = nullptr;
    if (status == JNI_OK) {
        jclass graph = env->FindClass("graph");
        if (graph != 0) {
            jmethodID graphID = env->GetMethodID(graph, "<init>", "(J)V");
            graphObj = env->NewObject(graph, graphID, address);
            if (graphObj == 0) { std::cout << "Create graph object failed." << std::endl;}
        } else { std::cout << "Find Class graph Failed! "; }
    }

    jclass algo = nullptr;
    jobject algoObj = nullptr;

    /** call algo.Init() function. */
    int tvnum = g->GetVerticesNum();
    std::vector<double> result(tvnum);
    g->set_presult_on_vertex(&result);

    if (status == JNI_OK) {
        algo = env->FindClass("algo");
        if (algo != 0) {
            jmethodID algoID = env->GetMethodID(algo, "<init>", "(Lgraph;)V");
            algoObj = env->NewObject(algo, algoID, graphObj);
            if (algoObj == 0) { std::cout << "Create algo object failed." << std::endl;}

            jmethodID InitID = env->GetMethodID(algo, "Init", "(Lgraph;)V");
            if (InitID != 0) {
                env-> CallObjectMethod(algoObj, InitID, graphObj);
            }
        } else { std::cout << "Find Class algo Failed! "; }
    }

    /** call algo.PEval() function. */
    if (status == JNI_OK) {
        if (algo != 0) {
            if (algoObj == 0) { std::cout << "Create algo object failed." << std::endl; }

            jmethodID PEvalID = env->GetMethodID(algo, "PEval", "(Lgraph;J)V");
            if (PEvalID != 0) {
                env->CallObjectMethod(algoObj, PEvalID, graphObj, 4);
            }
        } else { std::cout << "Find Class algo Failed! "; }
    }

    /** output result */
    std::string outputFile = argv[2];
    std::ofstream fout;
    fout.open(outputFile);

    auto inner_vertices = g->InnerVertices();
    for (auto &v : inner_vertices) {
        unsigned vid = v.vid();
        const double result = g->GetPResult(v);
        fout << vid << "\t" << result << "\n";
    }

    fout.close();
    std::cout << "result output: " << outputFile << std::endl;

}