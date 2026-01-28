#pragma once

#include "../core/Map.hpp"
#include <string>

class MapIO {
public:
    static bool saveMap(const Map& map, const std::string& filename);
    static std::unique_ptr<Map> loadMap(const std::string& filename);
    static bool saveMapText(const Map& map, const std::string& filename);
    static std::unique_ptr<Map> loadMapText(const std::string& filename);

private:
    static char terrainToChar(TerrainType terrain);
    static TerrainType charToTerrain(char c);
    static bool validateMap(const Map& map);
};
