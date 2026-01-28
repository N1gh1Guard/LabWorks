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

    std::vector<std::vector<double>> heightMap(params.rows, std::vector<double>(params.cols));
    std::vector<std::vector<double>> moistureMap(params.rows, std::vector<double>(params.cols));
    std::vector<std::vector<double>> temperatureMap(params.rows, std::vector<double>(params.cols));

    generateHeightMap(heightMap, params);
    generateMoistureMap(moistureMap, heightMap, params);
    generateTemperatureMap(temperatureMap, heightMap, params);

    normalizeMap(heightMap, params.rows, params.cols);

    if (params.useErosion) {
        applyErosion(heightMap, params.erosionIterations, params.rows, params.cols);
        normalizeMap(heightMap, params.rows, params.cols);
    }

    if (params.smoothCoastlines) {
        smoothCoastlines(heightMap, params);
    }
    
    normalizeMap(moistureMap, params.rows, params.cols);
    normalizeMap(temperatureMap, params.rows, params.cols);

    for (int row = 0; row < params.rows; ++row) {
        for (int col = 0; col < params.cols; ++col) {
            double height = heightMap[row][col];
            double moisture = moistureMap[row][col];
            double temperature = temperatureMap[row][col];
            
            TerrainType terrain = determineBiome(height, moisture, temperature, params);
            map->setCellTerrain(row, col, terrain);
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

double MapGenerator::ridgedNoise(double x, double y, unsigned int seed, double scale) {
    double value = fractalNoise(x, y, seed, scale);
    value = std::abs(value - 0.5) * 2.0;
    return value;
}

double MapGenerator::voronoiNoise(double x, double y, unsigned int seed, double scale) {
    int cellX = static_cast<int>(std::floor(x * scale));
    int cellY = static_cast<int>(std::floor(y * scale));
    
    double minDist = 1.0;
    
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            int nx = cellX + dx;
            int ny = cellY + dy;
            
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
    
    return std::min(1.0, minDist * 0.3);
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

double MapGenerator::smoothstep(double t) {
    return t * t * (3.0 - 2.0 * t);
}

void MapGenerator::getGradient(int ix, int iy, unsigned int seed, double& gx, double& gy) {
    unsigned int hash = seed;
    hash ^= static_cast<unsigned int>(ix) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    hash ^= static_cast<unsigned int>(iy) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    
    double angle = static_cast<double>(hash % 628) / 100.0;
    gx = std::cos(angle);
    gy = std::sin(angle);
}

double MapGenerator::dotGridGradient(int ix, int iy, double x, double y, unsigned int seed) {
    double gx, gy;
    getGradient(ix, iy, seed, gx, gy);
    
    double dx = x - static_cast<double>(ix);
    double dy = y - static_cast<double>(iy);
    
    return dx * gx + dy * gy;
}

double MapGenerator::perlinNoise(double x, double y, unsigned int seed) {
    int x0 = static_cast<int>(std::floor(x));
    int x1 = x0 + 1;
    int y0 = static_cast<int>(std::floor(y));
    int y1 = y0 + 1;
    
    double sx = smoothstep(x - x0);
    double sy = smoothstep(y - y0);
    
    double n0 = dotGridGradient(x0, y0, x, y, seed);
    double n1 = dotGridGradient(x1, y0, x, y, seed);
    double ix0 = n0 * (1.0 - sx) + n1 * sx;
    
    n0 = dotGridGradient(x0, y1, x, y, seed);
    n1 = dotGridGradient(x1, y1, x, y, seed);
    double ix1 = n0 * (1.0 - sx) + n1 * sx;
    
    double value = ix0 * (1.0 - sy) + ix1 * sy;
    
    return (value + 1.0) * 0.5;
}

double MapGenerator::improvedFractalNoise(double x, double y, unsigned int seed, double scale, int octaves) {
    double value = 0.0;
    double amplitude = 1.0;
    double frequency = 1.0;
    double maxValue = 0.0;

    for (int i = 0; i < octaves; ++i) {
        value += perlinNoise(x * frequency * scale, y * frequency * scale, seed + i * 1000) * amplitude;
        maxValue += amplitude;
        amplitude *= 0.5;
        frequency *= 2.0;
    }

    return value / maxValue;
}

void MapGenerator::domainWarp(double& x, double& y, unsigned int seed, double strength, double scale) {
    double warpX = perlinNoise(x * scale, y * scale, seed + 10000) * 2.0 - 1.0;
    double warpY = perlinNoise(x * scale, y * scale, seed + 20000) * 2.0 - 1.0;
    
    x += warpX * strength;
    y += warpY * strength;
}

void MapGenerator::generateHeightMap(std::vector<std::vector<double>>& heightMap, 
                                      const ProceduralGenerationParams& params) {
    unsigned int seed = params.seed;
    if (seed == 0) {
        std::random_device rd;
        seed = rd();
    }
    
    for (int row = 0; row < params.rows; ++row) {
        for (int col = 0; col < params.cols; ++col) {
            double x = static_cast<double>(col);
            double y = static_cast<double>(row);
            
            if (params.useDomainWarping) {
                double wx = x, wy = y;
                domainWarp(wx, wy, seed, params.domainWarpStrength, params.heightNoiseScale * 0.5);
                x = wx;
                y = wy;
            }
            
            double height1 = improvedFractalNoise(x, y, seed, params.heightNoiseScale, params.noiseOctaves);
            double height2 = ridgedNoise(x, y, seed + 5000, params.heightNoiseScale * 1.2);
            double height3 = voronoiNoise(x, y, seed + 10000, params.heightNoiseScale * 0.3);
            double height4 = improvedFractalNoise(x * 3.1, y * 3.1, seed + 15000, params.heightNoiseScale * 2.0, 3);
            
            double height = height1 * 0.4 + height2 * 0.25 + height3 * 0.2 + height4 * 0.15;
            
            heightMap[row][col] = height;
        }
    }
}

void MapGenerator::generateMoistureMap(std::vector<std::vector<double>>& moistureMap,
                                        const std::vector<std::vector<double>>& heightMap,
                                        const ProceduralGenerationParams& params) {
    unsigned int seed = params.seed + 30000;
    
    for (int row = 0; row < params.rows; ++row) {
        for (int col = 0; col < params.cols; ++col) {
            double x = static_cast<double>(col);
            double y = static_cast<double>(row);
            
            double moisture = improvedFractalNoise(x, y, seed, params.moistureNoiseScale, params.noiseOctaves);
            moisture = moisture * 0.7 + improvedFractalNoise(x * 2.3, y * 2.3, seed + 1000, params.moistureNoiseScale * 1.5, 3) * 0.3;
            
            double height = heightMap[row][col];
            if (height < params.seaLevel) {
                moisture = 0.7 + (height / params.seaLevel) * 0.3;
            } else {
                double heightAboveSea = height - params.seaLevel;
                double heightInfluence = -heightAboveSea * 0.25;
                moisture = std::min(1.0, std::max(0.0, moisture + heightInfluence));
            }
            
            moistureMap[row][col] = moisture;
        }
    }
}

void MapGenerator::generateTemperatureMap(std::vector<std::vector<double>>& temperatureMap,
                                           const std::vector<std::vector<double>>& heightMap,
                                           const ProceduralGenerationParams& params) {
    unsigned int seed = params.seed + 40000;
    
    for (int row = 0; row < params.rows; ++row) {
        for (int col = 0; col < params.cols; ++col) {
            double x = static_cast<double>(col);
            double y = static_cast<double>(row);
            
            double temperature = improvedFractalNoise(x, y, seed, params.temperatureNoiseScale, params.noiseOctaves);
            temperature = temperature * 0.7 + improvedFractalNoise(x * 1.7, y * 1.7, seed + 2000, params.temperatureNoiseScale * 1.3, 3) * 0.3;
            
            double height = heightMap[row][col];
            if (height < params.seaLevel) {
                temperature = 0.4 + (height / params.seaLevel) * 0.2;
            } else {
                double heightAboveSea = height - params.seaLevel;
                double heightInfluence = heightAboveSea * 0.3;
                temperature = std::min(1.0, std::max(0.0, temperature - heightInfluence));
            }
            
            double latitudeInfluence = static_cast<double>(row) / params.rows;
            temperature = temperature * 0.8 + latitudeInfluence * 0.2;
            
            temperatureMap[row][col] = temperature;
        }
    }
}

void MapGenerator::applyErosion(std::vector<std::vector<double>>& heightMap, 
                                 int iterations, int rows, int cols) {
    for (int iter = 0; iter < iterations; ++iter) {
        std::vector<std::vector<double>> newHeightMap = heightMap;
        
        for (int row = 1; row < rows - 1; ++row) {
            for (int col = 1; col < cols - 1; ++col) {
                double currentHeight = heightMap[row][col];
                double totalDiff = 0.0;
                int neighbors = 0;
                
                for (int dy = -1; dy <= 1; dy += 2) {
                    double neighborHeight = heightMap[row + dy][col];
                    double diff = currentHeight - neighborHeight;
                    if (diff > 0) {
                        totalDiff += diff * 0.25;
                        neighbors++;
                    }
                }
                for (int dx = -1; dx <= 1; dx += 2) {
                    double neighborHeight = heightMap[row][col + dx];
                    double diff = currentHeight - neighborHeight;
                    if (diff > 0) {
                        totalDiff += diff * 0.25;
                        neighbors++;
                    }
                }
                
                if (neighbors > 0) {
                    newHeightMap[row][col] = currentHeight - totalDiff / neighbors;
                }
            }
        }
        
        heightMap = newHeightMap;
    }
}

void MapGenerator::smoothCoastlines(std::vector<std::vector<double>>& heightMap,
                                     const ProceduralGenerationParams& params) {
    int radius = static_cast<int>(std::ceil(params.coastlineSmoothRadius));
    std::vector<std::vector<double>> smoothed = heightMap;
    
    for (int row = 0; row < params.rows; ++row) {
        for (int col = 0; col < params.cols; ++col) {
            double height = heightMap[row][col];
            bool isWater = height < params.seaLevel;
            
            int waterNeighbors = 0;
            int landNeighbors = 0;
            double totalHeight = 0.0;
            int count = 0;
            
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    int ny = row + dy;
                    int nx = col + dx;
                    
                    if (ny >= 0 && ny < params.rows && nx >= 0 && nx < params.cols) {
                        double dist = std::sqrt(dx * dx + dy * dy);
                        if (dist <= params.coastlineSmoothRadius) {
                            double neighborHeight = heightMap[ny][nx];
                            bool neighborIsWater = neighborHeight < params.seaLevel;
                            
                            if (neighborIsWater) waterNeighbors++;
                            else landNeighbors++;
                            
                            totalHeight += neighborHeight;
                            count++;
                        }
                    }
                }
            }
            
            if (isWater && landNeighbors > 0) {
                smoothed[row][col] = std::min(params.seaLevel, totalHeight / count);
            } else if (!isWater && waterNeighbors > 0) {
                smoothed[row][col] = std::max(params.seaLevel, totalHeight / count);
            }
        }
    }
    
    heightMap = smoothed;
}

void MapGenerator::normalizeMap(std::vector<std::vector<double>>& map, int rows, int cols) {
    double minVal = std::numeric_limits<double>::max();
    double maxVal = std::numeric_limits<double>::lowest();
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            minVal = std::min(minVal, map[row][col]);
            maxVal = std::max(maxVal, map[row][col]);
        }
    }
    
    if (std::abs(maxVal - minVal) < 1e-9) {
        return;
    }
    
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            map[row][col] = (map[row][col] - minVal) / (maxVal - minVal);
        }
    }
}

TerrainType MapGenerator::determineBiome(double height, double moisture, double temperature,
                                          const ProceduralGenerationParams& params) {
    if (height < params.deepWaterLevel) {
        return TerrainType::DeepWater;
    } else if (height < params.seaLevel) {
        return TerrainType::Water;
    }
    
    double moistureNorm = moisture;
    double tempNorm = temperature;
    double biomeIndex = moistureNorm * 0.6 + tempNorm * 0.4;
    
    if (moistureNorm >= params.forestMoisture) {
        if (tempNorm > 0.25 && tempNorm < 0.85) {
            return TerrainType::Forest;
        }
        if (tempNorm <= 0.25) {
            return TerrainType::Grass;
        }
    }
    
    if (moistureNorm < params.desertMoisture) {
        return TerrainType::Sand;
    }
    
    if (moistureNorm >= params.desertMoisture && moistureNorm < params.grasslandMoisture) {
        if (tempNorm > params.hotThreshold && biomeIndex < 0.45) {
            return TerrainType::Sand;
        }
        return TerrainType::Grass;
    }
    
    if (moistureNorm >= params.grasslandMoisture && moistureNorm < params.forestMoisture) {
        if (tempNorm < params.coldThreshold && moistureNorm < params.grasslandMoisture + 0.15) {
            return TerrainType::Sand;
        }
        return TerrainType::Grass;
    }
    
    return TerrainType::Grass;
}
