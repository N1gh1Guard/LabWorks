#include "Graph.hpp"
#include "Exceptions.hpp"
#include <algorithm>

Graph::Graph() : numVertices_(0), numEdges_(0) {
}

Graph::Graph(int numVertices) : numVertices_(numVertices), numEdges_(0) {
    if (numVertices <= 0) {
        THROW_GRAPH_EXCEPTION("Number of vertices must be positive");
    }
    adjacencyList_.resize(numVertices);
    vertexPassable_.resize(numVertices, true);
}

int Graph::getNumVertices() const {
    return numVertices_;
}

int Graph::getNumEdges() const {
    return numEdges_;
}

int Graph::addVertex(const Cell& cell) {
    int index = numVertices_;
    numVertices_++;
    adjacencyList_.resize(numVertices_);
    vertexPassable_.resize(numVertices_);
    vertexPassable_[index] = cell.isPassable();
    return index;
}

void Graph::addEdge(int from, int to, double weight) {
    if (!isValidVertex(from) || !isValidVertex(to)) {
        THROW_GRAPH_INVALID_VERTEX((from < 0 || from >= numVertices_) ? from : to, numVertices_);
    }
    if (from == to) {
        return;
    }
    if (weight < 0) {
        THROW_GRAPH_INVALID_EDGE(weight);
    }

    if (hasEdge(from, to)) {
        return;
    }
    
    adjacencyList_[from].push_back(Edge(to, weight));
    adjacencyList_[to].push_back(Edge(from, weight));
    numEdges_ += 1;
}

const std::list<Graph::Edge>& Graph::getAdjacencyList(int vertex) const {
    if (!isValidVertex(vertex)) {
        THROW_GRAPH_INVALID_VERTEX(vertex, numVertices_);
    }
    return adjacencyList_[vertex];
}

bool Graph::hasEdge(int from, int to) const {
    if (!isValidVertex(from) || !isValidVertex(to)) {
        return false;
    }
    
    const auto& adjList = adjacencyList_[from];
    return std::any_of(adjList.begin(), adjList.end(),
                       [to](const Edge& edge) { return edge.to == to; });
}

double Graph::getEdgeWeight(int from, int to) const {
    if (!isValidVertex(from) || !isValidVertex(to)) {
        return -1.0;
    }
    
    const auto& adjList = adjacencyList_[from];
    for (const auto& edge : adjList) {
        if (edge.to == to) {
            return edge.weight;
        }
    }
    
    return -1.0;
}

bool Graph::isValidVertex(int vertex) const {
    return vertex >= 0 && vertex < numVertices_;
}

bool Graph::isVertexPassable(int vertex) const {
    if (!isValidVertex(vertex)) {
        return false;
    }
    return vertexPassable_[vertex];
}
