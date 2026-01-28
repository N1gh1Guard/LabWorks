#pragma once

#include "../core/Map.hpp"
#include "../core/TerrainType.hpp"
#include <random>
#include <map>
#include <memory>

struct RandomGenerationParams {
    int rows;
    int cols;
    std::map<TerrainType, double> probabilities;
    unsigned int seed;

    RandomGenerationParams(int rows = 20, int cols = 20)
        : rows(rows), cols(cols), seed(0) {
        probabilities[TerrainType::Grass] = 0.4;
        probabilities[TerrainType::Sand] = 0.2;
        probabilities[TerrainType::Water] = 0.15;
        probabilities[TerrainType::DeepWater] = 0.1;
        probabilities[TerrainType::Forest] = 0.15;
    }
};

struct ProceduralGenerationParams {
    int rows;
    int cols;
    unsigned int seed;
    double noiseScale;
    double waterThreshold;
    double deepWaterThreshold;
    double forestThreshold;
    double sandThreshold;

    ProceduralGenerationParams(int rows = 20, int cols = 20)
        : rows(rows), cols(cols), seed(0), noiseScale(0.1),
          waterThreshold(0.4), deepWaterThreshold(0.2),
          forestThreshold(0.7), sandThreshold(0.5) {
    }
};

class MapGenerator {
public:
    static std::unique_ptr<Map> generateRandom(const RandomGenerationParams& params);
    static std::unique_ptr<Map> generateProcedural(const ProceduralGenerationParams& params);

private:
    static double simpleNoise(double x, double y, unsigned int seed);
    static double simpleNoise(int x, int y, unsigned int seed);
    static double fractalNoise(double x, double y, unsigned int seed, double scale, int octaves = 4);
    static double ridgedNoise(double x, double y, unsigned int seed, double scale);
    static double voronoiNoise(double x, double y, unsigned int seed, double scale);
    static TerrainType valueToTerrain(double value, const ProceduralGenerationParams& params);
    static void normalizeProbabilities(std::map<TerrainType, double>& probabilities);
    static TerrainType selectTerrainType(
        const std::map<TerrainType, double>& probabilities,
        std::mt19937& rng);
};
