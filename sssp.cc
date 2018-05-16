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