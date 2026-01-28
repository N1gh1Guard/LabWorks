#pragma once

#include "Cell.hpp"
#include "Graph.hpp"
#include <vector>
#include <memory>

class Map {
public:
    Map(int rows, int cols);
    ~Map() = default;

    int getRows() const;
    int getCols() const;
    const Cell& getCell(int row, int col) const;
    Cell& getCell(int row, int col);
    void setCellTerrain(int row, int col, TerrainType terrain);
    bool isValid(int row, int col) const;

    std::vector<std::pair<int, int>> getNeighbors(int row, int col) const;
    std::vector<std::pair<int, int>> getNeighbors8(int row, int col) const;

    std::unique_ptr<Graph> generateGraph(bool use8Connectivity = false) const;

    int cellToVertexIndex(int row, int col) const;
    std::pair<int, int> vertexIndexToCell(int vertexIndex) const;

private:
    int rows_;
    int cols_;
    std::vector<std::vector<Cell>> cells_;
};
