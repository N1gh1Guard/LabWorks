#include "Map.hpp"
#include "Exceptions.hpp"
#include <algorithm>

Map::Map(int rows, int cols) : rows_(rows), cols_(cols) {
    if (rows <= 0 || cols <= 0) {
        THROW_MAP_INVALID_DIMENSIONS(rows, cols);
    }
    
    cells_.resize(rows);
    for (int i = 0; i < rows; ++i) {
        cells_[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            cells_[i][j] = Cell(i, j, TerrainType::Grass);
        }
    }
}

int Map::getRows() const {
    return rows_;
}

int Map::getCols() const {
    return cols_;
}

const Cell& Map::getCell(int row, int col) const {
    if (!isValid(row, col)) {
        THROW_MAP_OUT_OF_BOUNDS(row, col);
    }
    return cells_[row][col];
}

Cell& Map::getCell(int row, int col) {
    if (!isValid(row, col)) {
        THROW_MAP_OUT_OF_BOUNDS(row, col);
    }
    return cells_[row][col];
}

void Map::setCellTerrain(int row, int col, TerrainType terrain) {
    if (!isValid(row, col)) {
        THROW_MAP_OUT_OF_BOUNDS(row, col);
    }
    cells_[row][col].setTerrainType(terrain);
}

bool Map::isValid(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
}

std::vector<std::pair<int, int>> Map::getNeighbors(int row, int col) const {
    std::vector<std::pair<int, int>> neighbors;
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    
    for (int i = 0; i < 4; ++i) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        if (isValid(newRow, newCol)) {
            neighbors.push_back({newRow, newCol});
        }
    }
    
    return neighbors;
}

std::vector<std::pair<int, int>> Map::getNeighbors8(int row, int col) const {
    std::vector<std::pair<int, int>> neighbors;
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (int i = 0; i < 8; ++i) {
        int newRow = row + directions[i][0];
        int newCol = col + directions[i][1];
        if (isValid(newRow, newCol)) {
            neighbors.push_back({newRow, newCol});
        }
    }
    
    return neighbors;
}

std::unique_ptr<Graph> Map::generateGraph(bool use8Connectivity) const {
    int numVertices = rows_ * cols_;
    auto graph = std::make_unique<Graph>(numVertices);
    
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            const Cell& cell = cells_[row][col];
            if (!cell.isPassable()) {
                continue;
            }
            
            int fromVertex = cellToVertexIndex(row, col);
            std::vector<std::pair<int, int>> neighbors;
            if (use8Connectivity) {
                neighbors = getNeighbors8(row, col);
            } else {
                neighbors = getNeighbors(row, col);
            }
            
            for (const auto& neighbor : neighbors) {
                int neighborRow = neighbor.first;
                int neighborCol = neighbor.second;
                const Cell& neighborCell = cells_[neighborRow][neighborCol];
                
                if (neighborCell.isPassable()) {
                    int toVertex = cellToVertexIndex(neighborRow, neighborCol);
                    double weight = neighborCell.getTraversalTime();
                    graph->addEdge(fromVertex, toVertex, weight);
                }
            }
        }
    }
    
    return graph;
}

int Map::cellToVertexIndex(int row, int col) const {
    return row * cols_ + col;
}

std::pair<int, int> Map::vertexIndexToCell(int vertexIndex) const {
    int row = vertexIndex / cols_;
    int col = vertexIndex % cols_;
    return {row, col};
}
