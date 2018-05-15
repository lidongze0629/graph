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

JNIEXPORT jint JNICALL Java_graph_GetVerticesNum
        (JNIEnv *env, jobject thisObj, jlong ptrAddr) {
    //std::cout << "getPtr address: " << ptrAddr << std::endl;
    void *ptr = (void*)(uintptr_t)ptrAddr;
    //std::cout << "ptr address: " << ptr << std::endl;
    Graph *gg = (Graph *)ptr;
    jint i =  gg->GetVerticesNum();
    return i;
}

JNIEXPORT jobject JNICALL Java_graph_InnerVertices
        (JNIEnv *env, jobject thisObj, jlong prtAddr) {

    void *ptr = (void*)(uintptr_t)prtAddr;
    Graph *gg = (Graph *)ptr;

    auto inner_vertices = gg->InnerVertices();
    auto beginaddr = inner_vertices.begin();
    auto endaddr = inner_vertices.end();
    jlong vbegin = (jlong)(uintptr_t)(&(*beginaddr));
    jlong vend = (jlong)(uintptr_t)(&(*endaddr));
    jint size = sizeof(Vertex);

    jclass IteratorPair = env->FindClass("IteratorPair");
    jmethodID initID = env->GetMethodID(IteratorPair, "<init>", "(JJI)V");
    jobject IteratorPairObj = env->NewObject(IteratorPair, initID, vbegin, vend, size);
    if (IteratorPairObj == 0) { std::cout << "Create IteratorPair object failed." << std::endl;}

    return IteratorPairObj;
}

JNIEXPORT void JNICALL Java_graph_SetAutoPResult
(JNIEnv *env, jobject thisObj, jlong ptrAddr, jlong vertexPtrAddr, jdouble r, jboolean init) {

    //void *ptr = (void*)(uintptr_t)vertexPtrAddr;
    //Vertex *vptr = (Vertex *)ptr;
    //std::cout << vptr->vid() << std::endl;
}

int main(int argc, char **argv) {

    if (argc != 3) { std::cout << "./graph-engine <query> <path_to_output_file>" << std::endl; exit(0); }

    std::unique_ptr<Graph> g(new Graph);

    /** load data, init graph */
    std::vector<Edge> edges;
    std::vector<Vertex> vertices;
    g->LoadFromFile (edges, vertices, "../data/twitter.v", "../data/twitter.e");
    g->InitGraph (vertices, edges);

    /** init PResult on vertex. */
    int tvnum = g->GetVerticesNum();
    std::vector<double> result(tvnum);
    g->set_presult_on_vertex(&result);

    auto inner_vertices = g->InnerVertices();
    for (auto &v : inner_vertices) {
        g->SetPResult(v, std::numeric_limits<double>::max(), true);
    }

    /** algo sssp. */
    std::string source = argv[1];
    unsigned query = std::stoi(source);
    std::cout << "For SSSP, Query source = " << query << std::endl;

    std::priority_queue<std::pair<double, unsigned>> heap;
    std::vector<bool> visited(tvnum, false);

    unsigned source_lid = query;
    double source_distance = 0.0;
    g->SetPResult(source_lid, source_distance);
    heap.push(std::make_pair(0, source_lid));

    while (!heap.empty()) {
        unsigned u = heap.top().second;
        heap.pop();

        if (visited[u]) continue;
        visited[u] = true;
        //temp.push_back(u);

        auto es = g->GetOutgoingEdgesLid(u);
        for (auto &iter : es) {
            unsigned v = iter.dst();
            double len = g->GetData(iter);

            double distv = g->GetPResult(g->GetVertexByLid(v));
            double distu = g->GetPResult(g->GetVertexByLid(u));

            if (distv > distu + len) {
                distv = distu + len;
                g->SetPResult(g->GetVertexByLid(v), distv);
                heap.push(std::make_pair(-distv, v));
            }
        }
    }

    /** output result */
    std::string outputFile = argv[2];
    std::ofstream fout;
    fout.open(outputFile);

    for (auto &v : inner_vertices) {
        unsigned vid = v.vid();
        const double result = g->GetPResult(v);
        fout << vid << "\t" << result << "\n";
    }

    fout.close();
    std::cout << "result output: " << outputFile << std::endl;

}















/*JavaVM *jvm = nullptr;
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
    if (status == JNI_OK) {

        jclass graph = env->FindClass("graph");
        jmethodID graphID = nullptr;
        jobject graphObj = nullptr;
        if (graph != 0) {
            graphID = env->GetMethodID(graph, "<init>", "()V");
            graphObj = env->NewObject(graph, graphID);
            if (graphObj == 0) { std::cout << "Create graph object failed." << std::endl;}

            jmethodID setPtrID = env->GetMethodID(graph, "setPtr", "(J)V");
            if (setPtrID != 0) {
                jlong address = (jlong)(uintptr_t)g.get();
                std::cout << "setPtr address: " << address << std::endl;
                env -> CallObjectMethod(graphObj, setPtrID, address);
            }
        } else {
            std::cout << "find class failed" << std::endl;
        }

        jclass algo = env->FindClass("algo");
        if (algo != 0) {
            jmethodID algoID = env->GetMethodID(algo, "<init>", "(Lgraph;)V");
            jobject algoObj = env->NewObject(algo, algoID, graphObj);
            if (algoObj == 0) { std::cout << "Create algo object failed." << std::endl;}

            jmethodID getPtrID = env->GetMethodID(algo, "getPtr", "()V");
            if (getPtrID != 0) {
                env-> CallObjectMethod(algoObj, getPtrID);
            }
        }
    }*/