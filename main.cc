#include <iostream>
#include <cstring>
#include <memory>
#include <jni.h>

#include "graph/edge.h"
#include "graph/vertex.h"
#include "graph/graph.h"

using grape::Edge;
using grape::Vertex;
using grape::Graph;

JNIEXPORT jint JNICALL Java_graph_GetVerticesNum
        (JNIEnv *env, jobject thisObj, jlong ptrAddr) {

    std::cout << "getPtr address: " << ptrAddr << std::endl;
    void *ptr = (void*)(uintptr_t)ptrAddr;
    Graph *gg = (Graph *)ptr;
    jint i =  gg->GetVerticesNum();
    return i;
}

int main(int argc, char *argv[]) {

    std::unique_ptr<Graph> g(new Graph);
    g->LoadFromFile("../data/test.v", "../data/test.e");

    std::cout << "g address: " << g.get() << std::endl;

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
    }


}