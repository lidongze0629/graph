#include "graph.h"

using grape::Edge;
using grape::Vertex;
using grape::Graph;
using grape::IteratorPair;

JNIEXPORT jlong JNICALL Java_graph_GetVertexBylid
        (JNIEnv *env, jobject thisObj, jlong ptrAddr, jlong lid) {
    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

    unsigned vlid = (unsigned)lid;

    Vertex v = gg->GetVertexByLid(vlid);
    std::cout << "v: " << v << std::endl;

    long vvptr = (long)&v;
    void *vtempptr = (void*)(uintptr_t)vvptr;
    Vertex *vvv = (Vertex *)vtempptr;
    std::cout << "vagain: " << *vvv << std::endl;

    jlong vptr  = 10;
    return vptr;

}

JNIEXPORT jdouble JNICALL Java_graph_GetPResult
        (JNIEnv *env, jobject object, jlong ptrAddr, jlong lid) {


    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

    unsigned vlid = (unsigned)lid;
    jdouble result = gg->GetPResult(vlid);

    return result;
}

JNIEXPORT jdouble JNICALL Java_graph_GetData
        (JNIEnv *env, jobject thisObj, jlong ptrAddr, jlong edgeptrAddr) {

    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

    void *edgeptr = (void*)(uintptr_t)edgeptrAddr;
    Edge *edge = (Edge *)edgeptr;

    jdouble len = (jdouble)gg->GetData(*edge);
    return len;
}

JNIEXPORT jint JNICALL Java_graph_GetVerticesNum
        (JNIEnv *env, jobject thisObj, jlong ptrAddr) {

    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

    jint i =  gg->GetVerticesNum();
    return i;
}

JNIEXPORT jobject JNICALL Java_graph_InnerVertices
            (JNIEnv *env, jobject thisObj, jlong ptrAddr) {

    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

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

JNIEXPORT void JNICALL Java_graph_SetPResult__JJDZ
            (JNIEnv *env, jobject thisObj, jlong ptrAddr, jlong vertexPtrAddr, jdouble r, jboolean init){

    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

    void *vptr = (void*)(uintptr_t)vertexPtrAddr;
    Vertex *vertex = (Vertex *)vptr;

    gg->SetPResult(*vertex, r, init);
}

JNIEXPORT void JNICALL Java_graph_SetPResult__JJD
            (JNIEnv *env, jobject thisObj, jlong ptrAddr, jlong lid, jdouble r) {

    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

    unsigned vlid = (unsigned)lid;
    double value = (double)r;

    gg->SetPResult(vlid, value);
}


JNIEXPORT jobject JNICALL Java_graph_GetOutgoingEdgesLid
        (JNIEnv *env, jobject thisObj, jlong ptrAddr, jlong lid) {

    Graph *gg = (Graph *)(void*)(uintptr_t)ptrAddr;

    unsigned vlid = (unsigned)lid;
    auto es = gg->GetOutgoingEdgesLid(vlid);
    auto beginaddr = es.begin();
    auto endaddr = es.end();

    jlong ebegin = (jlong)(uintptr_t)(&(*beginaddr));
    jlong eend = (jlong)(uintptr_t)(&(*endaddr));
    jint size = sizeof(Edge);

    jclass IteratorPair = env->FindClass("IteratorPair");
    jmethodID initID = env->GetMethodID(IteratorPair, "<init>", "(JJI)V");
    jobject IteratorPairObj = env->NewObject(IteratorPair, initID, ebegin, eend, size);
    if (IteratorPairObj == 0) { std::cout << "Create IteratorPair object failed." << std::endl;}

    return IteratorPairObj;
}