#pragma once

#include <string>
#include <limits>

enum class TerrainType {
    Grass,
    Sand,
    Water,
    DeepWater,
    Forest  // Непроходим
};

class TerrainTypeHelper {
public:
    static double getDefaultTraversalTime(TerrainType type) {
        switch (type) {
            case TerrainType::Grass:
                return 1.0;
            case TerrainType::Sand:
                return 1.5;
            case TerrainType::Water:
                return 2.0;
            case TerrainType::DeepWater:
                return 3.0;
            case TerrainType::Forest:
                return std::numeric_limits<double>::infinity();
            default:
                return 1.0;
        }
    }

    static bool isPassable(TerrainType type) {
        return type != TerrainType::Forest;
    }

    static std::string toString(TerrainType type) {
        switch (type) {
            case TerrainType::Grass:
                return "Grass";
            case TerrainType::Sand:
                return "Sand";
            case TerrainType::Water:
                return "Water";
            case TerrainType::DeepWater:
                return "DeepWater";
            case TerrainType::Forest:
                return "Forest";
            default:
                return "Unknown";
        }
    }
};
