#include "Cell.hpp"
#include <limits>

Cell::Cell() : row_(0), col_(0), terrain_(TerrainType::Grass), traversalTime_(1.0) {}

Cell::Cell(int row, int col, TerrainType terrain, double traversalTime)
    : row_(row), col_(col), terrain_(terrain) {
    if (traversalTime < 0.0) {
        traversalTime_ = TerrainTypeHelper::getDefaultTraversalTime(terrain);
    } else {
        traversalTime_ = traversalTime;
    }
}

int Cell::getRow() const {
    return row_;
}

int Cell::getCol() const {
    return col_;
}

TerrainType Cell::getTerrainType() const {
    return terrain_;
}

void Cell::setTerrainType(TerrainType terrain) {
    terrain_ = terrain;
    traversalTime_ = TerrainTypeHelper::getDefaultTraversalTime(terrain);
}

double Cell::getTraversalTime() const {
    return traversalTime_;
}

void Cell::setTraversalTime(double time) {
    traversalTime_ = time;
}

bool Cell::isPassable() const {
    return TerrainTypeHelper::isPassable(terrain_);
}

bool Cell::operator==(const Cell& other) const {
    return row_ == other.row_ && col_ == other.col_;
}

bool Cell::operator!=(const Cell& other) const {
    return !(*this == other);
}
