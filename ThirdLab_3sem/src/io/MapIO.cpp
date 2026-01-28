#include "MapIO.hpp"
#include "../core/Exceptions.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>

bool MapIO::saveMap(const Map& map, const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            THROW_FILE_WRITE_EXCEPTION(filename);
        }

        file << "{\n";
        file << "  \"rows\": " << map.getRows() << ",\n";
        file << "  \"cols\": " << map.getCols() << ",\n";
        file << "  \"cells\": [\n";

        for (int row = 0; row < map.getRows(); ++row) {
            file << "    [";
            for (int col = 0; col < map.getCols(); ++col) {
                const Cell& cell = map.getCell(row, col);
                TerrainType terrain = cell.getTerrainType();
                
                std::string terrainStr;
                switch (terrain) {
                    case TerrainType::Grass:
                        terrainStr = "Grass";
                        break;
                    case TerrainType::Sand:
                        terrainStr = "Sand";
                        break;
                    case TerrainType::Water:
                        terrainStr = "Water";
                        break;
                    case TerrainType::DeepWater:
                        terrainStr = "DeepWater";
                        break;
                    case TerrainType::Forest:
                        terrainStr = "Forest";
                        break;
                }

                file << "{\"row\":" << row << ",\"col\":" << col 
                     << ",\"terrain\":\"" << terrainStr << "\""
                     << ",\"time\":" << cell.getTraversalTime() << "}";
                
                if (col < map.getCols() - 1) {
                    file << ",";
                }
            }
            file << "]";
            if (row < map.getRows() - 1) {
                file << ",";
            }
            file << "\n";
        }

        file << "  ]\n";
        file << "}\n";

        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при сохранении карты: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<Map> MapIO::loadMap(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            THROW_FILE_NOT_FOUND(filename);
        }

        std::string line;
        int rows = 0, cols = 0;
        bool foundRows = false, foundCols = false;

        while (std::getline(file, line)) {
            if (line.find("\"rows\"") != std::string::npos) {
                std::istringstream iss(line);
                std::string token;
                while (iss >> token) {
                    if (token.find_first_of("0123456789") != std::string::npos) {
                        rows = std::stoi(token);
                        foundRows = true;
                        break;
                    }
                }
            }
            if (line.find("\"cols\"") != std::string::npos) {
                std::istringstream iss(line);
                std::string token;
                while (iss >> token) {
                    if (token.find_first_of("0123456789") != std::string::npos) {
                        cols = std::stoi(token);
                        foundCols = true;
                        break;
                    }
                }
            }
            if (foundRows && foundCols) break;
        }

        if (!foundRows || !foundCols || rows <= 0 || cols <= 0) {
            std::cerr << "Ошибка: неверный формат файла или невалидные размеры" << std::endl;
            return nullptr;
        }

        auto map = std::make_unique<Map>(rows, cols);

        int currentRow = 0;
        file.seekg(0);
        bool inCells = false;

        while (std::getline(file, line) && currentRow < rows) {
            if (line.find("\"cells\"") != std::string::npos) {
                inCells = true;
                continue;
            }
            if (!inCells) continue;

            if (line.find("[") != std::string::npos) {
                std::string cellData = line;
                size_t pos = 0;
                int currentCol = 0;

                while ((pos = cellData.find("{", pos)) != std::string::npos && currentCol < cols) {
                    size_t endPos = cellData.find("}", pos);
                    if (endPos == std::string::npos) break;

                    std::string cellStr = cellData.substr(pos, endPos - pos + 1);
                    
                    size_t terrainPos = cellStr.find("\"terrain\"");
                    if (terrainPos != std::string::npos) {
                        size_t start = cellStr.find("\"", terrainPos + 9) + 1;
                        size_t end = cellStr.find("\"", start);
                        std::string terrainStr = cellStr.substr(start, end - start);

                        TerrainType terrain;
                        if (terrainStr == "Grass") terrain = TerrainType::Grass;
                        else if (terrainStr == "Sand") terrain = TerrainType::Sand;
                        else if (terrainStr == "Water") terrain = TerrainType::Water;
                        else if (terrainStr == "DeepWater") terrain = TerrainType::DeepWater;
                        else if (terrainStr == "Forest") terrain = TerrainType::Forest;
                        else terrain = TerrainType::Grass;

                        map->setCellTerrain(currentRow, currentCol, terrain);
                    }

                    pos = endPos + 1;
                    currentCol++;
                }
                currentRow++;
            }
        }

        if (!validateMap(*map)) {
            std::cerr << "Ошибка: загруженная карта не прошла валидацию" << std::endl;
            return nullptr;
        }

        return map;
    } catch (const BaseException& e) {
        ExceptionTracker::getInstance().registerException(e);
        std::cerr << "Ошибка при загрузке карты: " << e.what() << std::endl;
        return nullptr;
    } catch (const std::exception& e) {
        BaseException baseEx(e.what());
        ExceptionTracker::getInstance().registerException(baseEx);
        std::cerr << "Ошибка при загрузке карты: " << e.what() << std::endl;
        return nullptr;
    }
}

bool MapIO::saveMapText(const Map& map, const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            THROW_FILE_WRITE_EXCEPTION(filename);
        }

        file << map.getRows() << " " << map.getCols() << "\n";

        for (int row = 0; row < map.getRows(); ++row) {
            for (int col = 0; col < map.getCols(); ++col) {
                const Cell& cell = map.getCell(row, col);
                file << terrainToChar(cell.getTerrainType());
            }
            file << "\n";
        }

        file.close();
        return true;
    } catch (const BaseException& e) {
        ExceptionTracker::getInstance().registerException(e);
        std::cerr << "Ошибка при сохранении карты: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        BaseException baseEx(e.what());
        ExceptionTracker::getInstance().registerException(baseEx);
        std::cerr << "Ошибка при сохранении карты: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<Map> MapIO::loadMapText(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            THROW_FILE_NOT_FOUND(filename);
        }

        int rows, cols;
        file >> rows >> cols;

        if (rows <= 0 || cols <= 0) {
            std::cerr << "Ошибка: невалидные размеры карты" << std::endl;
            return nullptr;
        }

        auto map = std::make_unique<Map>(rows, cols);

        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (int row = 0; row < rows; ++row) {
            std::string line;
            if (!std::getline(file, line)) {
                std::cerr << "Ошибка: недостаточно данных в файле" << std::endl;
                return nullptr;
            }

            for (int col = 0; col < cols && col < static_cast<int>(line.length()); ++col) {
                TerrainType terrain = charToTerrain(line[col]);
                map->setCellTerrain(row, col, terrain);
            }
        }

        if (!validateMap(*map)) {
            std::cerr << "Ошибка: загруженная карта не прошла валидацию" << std::endl;
            return nullptr;
        }

        return map;
    } catch (const BaseException& e) {
        ExceptionTracker::getInstance().registerException(e);
        std::cerr << "Ошибка при загрузке карты: " << e.what() << std::endl;
        return nullptr;
    } catch (const std::exception& e) {
        BaseException baseEx(e.what());
        ExceptionTracker::getInstance().registerException(baseEx);
        std::cerr << "Ошибка при загрузке карты: " << e.what() << std::endl;
        return nullptr;
    }
}

char MapIO::terrainToChar(TerrainType terrain) {
    switch (terrain) {
        case TerrainType::Grass:
            return 'G';
        case TerrainType::Sand:
            return 'S';
        case TerrainType::Water:
            return 'W';
        case TerrainType::DeepWater:
            return 'D';
        case TerrainType::Forest:
            return 'F';
        default:
            return 'G';
    }
}

TerrainType MapIO::charToTerrain(char c) {
    switch (c) {
        case 'G':
        case 'g':
            return TerrainType::Grass;
        case 'S':
        case 's':
            return TerrainType::Sand;
        case 'W':
        case 'w':
            return TerrainType::Water;
        case 'D':
        case 'd':
            return TerrainType::DeepWater;
        case 'F':
        case 'f':
            return TerrainType::Forest;
        default:
            return TerrainType::Grass;
    }
}

bool MapIO::validateMap(const Map& map) {
    if (map.getRows() <= 0 || map.getCols() <= 0) {
        return false;
    }

    for (int row = 0; row < map.getRows(); ++row) {
        for (int col = 0; col < map.getCols(); ++col) {
            if (!map.isValid(row, col)) {
                return false;
            }
        }
    }

    return true;
}
