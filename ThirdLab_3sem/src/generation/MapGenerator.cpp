#include "MapGenerator.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

std::unique_ptr<Map> MapGenerator::generateRandom(const RandomGenerationParams& params) {
    auto map = std::make_unique<Map>(params.rows, params.cols);
    auto normalizedProbs = params.probabilities;
    normalizeProbabilities(normalizedProbs);

    std::mt19937 rng;
    if (params.seed == 0) {
        std::random_device rd;
        rng.seed(rd());
    } else {
        rng.seed(params.seed);
    }

    for (int row = 0; row < params.rows; ++row) {
        for (int col = 0; col < params.cols; ++col) {
            TerrainType terrain = selectTerrainType(normalizedProbs, rng);
            map->setCellTerrain(row, col, terrain);
        }
    }

    return map;
}

std::unique_ptr<Map> MapGenerator::generateProcedural(const ProceduralGenerationParams& params) {
    auto map = std::make_unique<Map>(params.rows, params.cols);

    unsigned int seed = params.seed;
    if (seed == 0) {
        std::random_device rd;
        seed = rd();
    }

    // Комбинация нескольких паттернов для более интересного результата
    for (int row = 0; row < params.rows; ++row) {
        for (int col = 0; col < params.cols; ++col) {
            double v1 = fractalNoise(col, row, seed, params.noiseScale);
            double v2 = voronoiNoise(col, row, seed + 1000, params.noiseScale * 0.5);
            double v3 = ridgedNoise(col, row, seed + 2000, params.noiseScale * 1.5);
            double noiseValue = (v1 * 0.5 + v2 * 0.3 + v3 * 0.2);
            map->setCellTerrain(row, col, valueToTerrain(noiseValue, params));
        }
    }

    return map;
}

double MapGenerator::simpleNoise(double x, double y, unsigned int seed) {
    int ix = static_cast<int>(std::floor(x));
    int iy = static_cast<int>(std::floor(y));
    
    unsigned int hash = seed;
    hash ^= static_cast<unsigned int>(ix) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= static_cast<unsigned int>(iy) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    
    double value = static_cast<double>(hash % 1000000) / 1000000.0;
    double fx = x - ix;
    double fy = y - iy;
    
    double n00 = value;
    double n10 = simpleNoise(ix + 1, iy, seed);
    double n01 = simpleNoise(ix, iy + 1, seed);
    double n11 = simpleNoise(ix + 1, iy + 1, seed);
    
    double nx0 = n00 * (1.0 - fx) + n10 * fx;
    double nx1 = n01 * (1.0 - fx) + n11 * fx;
    double result = nx0 * (1.0 - fy) + nx1 * fy;
    
    return result;
}

double MapGenerator::simpleNoise(int x, int y, unsigned int seed) {
    unsigned int hash = seed;
    hash ^= static_cast<unsigned int>(x) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= static_cast<unsigned int>(y) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    return static_cast<double>(hash % 1000000) / 1000000.0;
}

double MapGenerator::fractalNoise(double x, double y, unsigned int seed, double scale, int octaves) {
    double value = 0.0;
    double amplitude = 1.0;
    double frequency = 1.0;
    double maxValue = 0.0;

    for (int i = 0; i < octaves; ++i) {
        value += simpleNoise(x * frequency * scale, y * frequency * scale, seed + i) * amplitude;
        maxValue += amplitude;
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    return value / maxValue;
}

void MapGenerator::normalizeProbabilities(std::map<TerrainType, double>& probabilities) {
    double sum = 0.0;
    for (const auto& pair : probabilities) {
        sum += pair.second;
    }

    if (sum < 1e-6) {
        double uniformProb = 1.0 / probabilities.size();
        for (auto& pair : probabilities) {
            pair.second = uniformProb;
        }
        return;
    }

    for (auto& pair : probabilities) {
        pair.second /= sum;
    }
}

TerrainType MapGenerator::valueToTerrain(double value, const ProceduralGenerationParams& params) {
    if (value < params.deepWaterThreshold) {
        return TerrainType::DeepWater;
    } else if (value < params.waterThreshold) {
        return TerrainType::Water;
    } else if (value < params.sandThreshold) {
        return TerrainType::Sand;
    } else if (value < params.forestThreshold) {
        return TerrainType::Grass;
    } else {
        return TerrainType::Forest;
    }
}

double MapGenerator::ridgedNoise(double x, double y, unsigned int seed, double scale) {
    double value = fractalNoise(x, y, seed, scale);
    value = std::abs(value - 0.5) * 2.0; // Преобразуем в ridged форму
    return value;
}

double MapGenerator::voronoiNoise(double x, double y, unsigned int seed, double scale) {
    int cellX = static_cast<int>(std::floor(x * scale));
    int cellY = static_cast<int>(std::floor(y * scale));
    
    double minDist = 1.0;
    
    // Проверяем соседние ячейки Вороного
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = cellX + dx;
            int ny = cellY + dy;
            
            // Генерируем детерминированную точку внутри ячейки
            unsigned int hash = seed;
            hash ^= static_cast<unsigned int>(nx) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            hash ^= static_cast<unsigned int>(ny) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            
            double px = nx + static_cast<double>(hash % 1000) / 1000.0;
            hash = hash * 1103515245 + 12345;
            double py = ny + static_cast<double>(hash % 1000) / 1000.0;
            
            double dx2 = (x * scale - px);
            double dy2 = (y * scale - py);
            double dist2 = std::sqrt(dx2 * dx2 + dy2 * dy2);
            
            if (dist2 < minDist) {
                minDist = dist2;
            }
        }
    }
    
    return std::min(1.0, minDist * 0.3); // Нормализуем
}


TerrainType MapGenerator::selectTerrainType(
    const std::map<TerrainType, double>& probabilities,
    std::mt19937& rng) {
    
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    double randomValue = dist(rng);

    double cumulative = 0.0;
    for (const auto& pair : probabilities) {
        cumulative += pair.second;
        if (randomValue < cumulative) {
            return pair.first;
        }
    }

    return probabilities.rbegin()->first;
}
