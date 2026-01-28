#pragma once

#include <vector>
#include <queue>
#include <limits>
#include "../core/Graph.hpp"

struct PathResult {
    std::vector<int> path;
    double totalCost;
    bool found;
    
    PathResult() : totalCost(0.0), found(false) {}
};

class Pathfinder {
public:
    static PathResult findShortestPath(const Graph& graph, int start, int end);
    static std::vector<PathResult> findAllShortestPaths(const Graph& graph, int start, int end);
    static long long measureExecutionTime(const Graph& graph, int start, int end);

private:
    struct QueueNode {
        int vertex;
        double distance;
        
        bool operator>(const QueueNode& other) const {
            return distance > other.distance;
        }
    };
    
    static void dijkstra(const Graph& graph, int start, 
                        std::vector<double>& distances,
                        std::vector<std::vector<int>>& predecessors);
    
    static std::vector<int> reconstructPath(const std::vector<std::vector<int>>& predecessors,
                                           int start, int end);
    
    static void findAllPathsDFS(const std::vector<std::vector<int>>& predecessors,
                                int current, int start,
                                std::vector<int>& currentPath,
                                std::vector<std::vector<int>>& allPaths);
};
