#include "Pathfinder.hpp"
#include <chrono>
#include <algorithm>

PathResult Pathfinder::findShortestPath(const Graph& graph, int start, int end) {
    PathResult result;
    
    int numVertices = graph.getNumVertices();
    if (start < 0 || start >= numVertices ||
        end < 0 || end >= numVertices) {
        return result;
    }
    
    if (start == end) {
        result.path = {start};
        result.totalCost = 0.0;
        result.found = true;
        return result;
    }
    
    std::vector<double> distances(numVertices, std::numeric_limits<double>::infinity());
    std::vector<std::vector<int>> predecessors(numVertices);
    std::vector<bool> visited(numVertices, false);
    
    distances[start] = 0.0;
    
    std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<QueueNode>> pq;
    pq.push({start, 0.0});
    
    while (!pq.empty()) {
        QueueNode current = pq.top();
        pq.pop();
        
        int u = current.vertex;
        
        if (visited[u]) {
            continue;
        }
        
        visited[u] = true;
        
        if (u == end) {
            break;
        }
        
        const auto& neighbors = graph.getAdjacencyList(u);
        for (const auto& edge : neighbors) {
            int v = edge.to;
            double weight = edge.weight;
            
            if (!visited[v]) {
                double newDist = distances[u] + weight;
                
                const double absEpsilon = 1e-12;
                const double relEpsilon = 1e-14;
                const double maxDist = std::max(std::abs(newDist), std::abs(distances[v]));
                const double epsilon = absEpsilon + relEpsilon * std::max(1.0, maxDist);
                
                const double diff = newDist - distances[v];
                
                if (diff < -epsilon) {
                    distances[v] = newDist;
                    predecessors[v].clear();
                    predecessors[v].push_back(u);
                    pq.push({v, newDist});
                }
                else if (std::abs(diff) <= epsilon) {
                    bool found = false;
                    for (int pred : predecessors[v]) {
                        if (pred == u) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        predecessors[v].push_back(u);
                    }
                }
            }
        }
    }
    if (distances[end] != std::numeric_limits<double>::infinity()) {
        result.path = reconstructPath(predecessors, start, end);
        result.totalCost = distances[end];
        result.found = true;
    }
    
    return result;
}

std::vector<PathResult> Pathfinder::findAllShortestPaths(const Graph& graph, int start, int end) {
    std::vector<PathResult> results;
    
    int numVertices = graph.getNumVertices();
    if (start < 0 || start >= numVertices ||
        end < 0 || end >= numVertices) {
        return results;
    }
    
    if (start == end) {
        PathResult result;
        result.path = {start};
        result.totalCost = 0.0;
        result.found = true;
        results.push_back(result);
        return results;
    }
    
    std::vector<double> distances(numVertices, std::numeric_limits<double>::infinity());
    std::vector<std::vector<int>> predecessors(numVertices);
    std::vector<bool> visited(numVertices, false);
    
    distances[start] = 0.0;
    
    std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<QueueNode>> pq;
    pq.push({start, 0.0});
    
    while (!pq.empty()) {
        QueueNode current = pq.top();
        pq.pop();
        
        int u = current.vertex;
        
        if (visited[u]) {
            continue;
        }
        
        visited[u] = true;
        
        const auto& neighbors = graph.getAdjacencyList(u);
        for (const auto& edge : neighbors) {
            int v = edge.to;
            double weight = edge.weight;
            
            if (!visited[v]) {
                double newDist = distances[u] + weight;
                
                const double absEpsilon = 1e-12;
                const double relEpsilon = 1e-14;
                const double maxDist = std::max(std::abs(newDist), std::abs(distances[v]));
                const double epsilon = absEpsilon + relEpsilon * std::max(1.0, maxDist);
                
                const double diff = newDist - distances[v];
                
                if (diff < -epsilon) {
                    distances[v] = newDist;
                    predecessors[v].clear();
                    predecessors[v].push_back(u);
                    pq.push({v, newDist});
                }
                else if (std::abs(diff) <= epsilon) {
                    bool found = false;
                    for (int pred : predecessors[v]) {
                        if (pred == u) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        predecessors[v].push_back(u);
                    }
                }
            }
        }
    }
    
    if (distances[end] == std::numeric_limits<double>::infinity()) {
        return results;
    }
    
    std::vector<int> currentPath;
    std::vector<std::vector<int>> allPaths;
    
    findAllPathsDFS(predecessors, end, start, currentPath, allPaths);
    double minCost = distances[end];
    for (const auto& path : allPaths) {
        PathResult result;
        result.path = path;
        result.totalCost = minCost;
        result.found = true;
        results.push_back(result);
    }
    
    return results;
}

long long Pathfinder::measureExecutionTime(const Graph& graph, int start, int end) {
    auto startTime = std::chrono::high_resolution_clock::now();
    findShortestPath(graph, start, end);
    auto endTime = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    return duration.count();
}

void Pathfinder::dijkstra(const Graph& graph, int start,
                         std::vector<double>& distances,
                         std::vector<std::vector<int>>& predecessors) {
    int numVertices = graph.getNumVertices();
    distances.assign(numVertices, std::numeric_limits<double>::infinity());
    predecessors.assign(numVertices, std::vector<int>());
    std::vector<bool> visited(numVertices, false);
    
    distances[start] = 0.0;
    
    std::priority_queue<QueueNode, std::vector<QueueNode>, std::greater<QueueNode>> pq;
    pq.push({start, 0.0});
    
    while (!pq.empty()) {
        QueueNode current = pq.top();
        pq.pop();
        
        int u = current.vertex;
        
        if (visited[u]) {
            continue;
        }
        
        visited[u] = true;
        
        const auto& neighbors = graph.getAdjacencyList(u);
        for (const auto& edge : neighbors) {
            int v = edge.to;
            double weight = edge.weight;
            
            if (!visited[v]) {
                double newDist = distances[u] + weight;
                
                const double absEpsilon = 1e-12;
                const double relEpsilon = 1e-14;
                const double maxDist = std::max(std::abs(newDist), std::abs(distances[v]));
                const double epsilon = absEpsilon + relEpsilon * std::max(1.0, maxDist);
                
                const double diff = newDist - distances[v];
                
                if (diff < -epsilon) {
                    distances[v] = newDist;
                    predecessors[v].clear();
                    predecessors[v].push_back(u);
                    pq.push({v, newDist});
                }
                else if (std::abs(diff) <= epsilon) {
                    bool found = false;
                    for (int pred : predecessors[v]) {
                        if (pred == u) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        predecessors[v].push_back(u);
                    }
                }
            }
        }
    }
}

std::vector<int> Pathfinder::reconstructPath(const std::vector<std::vector<int>>& predecessors,
                                            int start, int end) {
    std::vector<int> path;
    
    if (predecessors[end].empty() && start != end) {
        return path;
    }
    
    int current = end;
    path.push_back(current);
    
    while (current != start) {
        if (predecessors[current].empty()) {
            path.clear();
            return path;
        }
        current = predecessors[current][0];
        path.push_back(current);
    }
    
    std::reverse(path.begin(), path.end());
    
    return path;
}

void Pathfinder::findAllPathsDFS(const std::vector<std::vector<int>>& predecessors,
                                 int current, int start,
                                 std::vector<int>& currentPath,
                                 std::vector<std::vector<int>>& allPaths) {
    currentPath.push_back(current);
    
    if (current == start) {
        std::vector<int> path = currentPath;
        std::reverse(path.begin(), path.end());
        allPaths.push_back(path);
    } else {
        for (int pred : predecessors[current]) {
            findAllPathsDFS(predecessors, pred, start, currentPath, allPaths);
        }
    }
    
    currentPath.pop_back();
}
