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
    
    double heightNoiseScale;
    double moistureNoiseScale;
    double temperatureNoiseScale;
    int noiseOctaves;
    
    double seaLevel;
    double deepWaterLevel;
    
    double desertMoisture;
    double grasslandMoisture;
    double forestMoisture;
    
    double coldThreshold;
    double hotThreshold;
    
    bool useDomainWarping;
    double domainWarpStrength;
    bool useErosion;
    int erosionIterations;
    bool smoothCoastlines;
    double coastlineSmoothRadius;

    ProceduralGenerationParams(int rows = 20, int cols = 20)
        : rows(rows), cols(cols), seed(0),
          heightNoiseScale(0.05), moistureNoiseScale(0.08), temperatureNoiseScale(0.06),
          noiseOctaves(6),
          seaLevel(0.35), deepWaterLevel(0.25),
          desertMoisture(0.3), grasslandMoisture(0.5), forestMoisture(0.7),
          coldThreshold(0.3), hotThreshold(0.7),
          useDomainWarping(true), domainWarpStrength(0.3),
          useErosion(true), erosionIterations(3),
          smoothCoastlines(true), coastlineSmoothRadius(1.5) {
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
    
    static double perlinNoise(double x, double y, unsigned int seed);
    static double smoothstep(double t);
    static double dotGridGradient(int ix, int iy, double x, double y, unsigned int seed);
    static void getGradient(int ix, int iy, unsigned int seed, double& gx, double& gy);
    static double improvedFractalNoise(double x, double y, unsigned int seed, double scale, int octaves);
    
    static void domainWarp(double& x, double& y, unsigned int seed, double strength, double scale);
    
    static void generateHeightMap(std::vector<std::vector<double>>& heightMap, 
                                   const ProceduralGenerationParams& params);
    static void generateMoistureMap(std::vector<std::vector<double>>& moistureMap,
                                     const std::vector<std::vector<double>>& heightMap,
                                     const ProceduralGenerationParams& params);
    static void generateTemperatureMap(std::vector<std::vector<double>>& temperatureMap,
                                        const std::vector<std::vector<double>>& heightMap,
                                        const ProceduralGenerationParams& params);
    
    static void normalizeMap(std::vector<std::vector<double>>& map, int rows, int cols);
    
    static void applyErosion(std::vector<std::vector<double>>& heightMap, 
                             int iterations, int rows, int cols);
    static void smoothCoastlines(std::vector<std::vector<double>>& heightMap,
                                  const ProceduralGenerationParams& params);
    
    static TerrainType determineBiome(double height, double moisture, double temperature,
                                       const ProceduralGenerationParams& params);
    
    static void normalizeProbabilities(std::map<TerrainType, double>& probabilities);
    static TerrainType selectTerrainType(
        const std::map<TerrainType, double>& probabilities,
        std::mt19937& rng);
};
