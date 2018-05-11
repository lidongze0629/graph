#include <iostream>

#include "edge.h"
#include "vertex.h"
#include "graph.h"

using grape::Edge;
using grape::Vertex;
using grape::Graph;

int main(int argc, char *argv[]) {

    Graph g;
    g.LoadFromFile("data/test.v", "data/test.e");

    for (int i = 0; i < g.GetVerticesNum(); i++) {
        std::vector <Edge> ie = g.GetIncomingEdges(i);
        for (auto iter = ie.cbegin(); iter != ie.cend(); iter++) {
            //std::cout << iter->src() << "->" << iter->dst() << "(" << iter->edata() << ")" << std::endl;
        }

        std::vector <Edge> oe = g.GetOutgoingEdges(i);
        for (auto iter = oe.cbegin(); iter != oe.cend(); iter++) {
            std::cout << iter->src() << "->" << iter->dst() << "(" << iter->edata() << ")" << std::endl;
        }
    }

}