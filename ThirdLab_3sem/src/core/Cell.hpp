#pragma once

#include "TerrainType.hpp"

class Cell {
public:
    Cell();
    Cell(int row, int col, TerrainType terrain, double traversalTime = -1.0);

    int getRow() const;
    int getCol() const;
    TerrainType getTerrainType() const;
    void setTerrainType(TerrainType terrain);
    double getTraversalTime() const;
    void setTraversalTime(double time);
    bool isPassable() const;

    bool operator==(const Cell& other) const;
    bool operator!=(const Cell& other) const;

private:
    int row_;
    int col_;
    TerrainType terrain_;
    double traversalTime_;
};
