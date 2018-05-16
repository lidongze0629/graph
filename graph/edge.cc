#include "edge.h"

using grape::Edge;

JNIEXPORT jlong JNICALL Java_Edge_dst
        (JNIEnv *env, jclass thisObj, jlong eptr){

    void *edgeptr = (void*)(uintptr_t)eptr;
    Edge *edge = (Edge *)edgeptr;

    unsigned v = edge->dst();
    jlong dst = (jlong)v;

    return dst;
}