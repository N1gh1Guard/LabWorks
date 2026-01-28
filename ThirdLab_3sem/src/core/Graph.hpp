#pragma once

#include <vector>
#include <list>
#include <utility>
#include "Cell.hpp"

class Graph {
public:
    struct Edge {
        int to;
        double weight;

        Edge(int to, double weight) : to(to), weight(weight) {}
    };

    Graph();
    explicit Graph(int numVertices);
    ~Graph() = default;

    int getNumVertices() const;
    int getNumEdges() const;
    int addVertex(const Cell& cell);
    void addEdge(int from, int to, double weight);
    const std::list<Edge>& getAdjacencyList(int vertex) const;
    bool hasEdge(int from, int to) const;
    double getEdgeWeight(int from, int to) const;
    bool isValidVertex(int vertex) const;
    bool isVertexPassable(int vertex) const;

private:
    int numVertices_;
    int numEdges_;
    std::vector<std::list<Edge>> adjacencyList_;
    std::vector<bool> vertexPassable_;
};
