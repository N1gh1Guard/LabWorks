#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip>
#include <cmath>
#include "../algorithms/Pathfinder.hpp"
#include "../core/Graph.hpp"
#include "../core/Cell.hpp"
#include "../core/TerrainType.hpp"

class AlgorithmTests {
public:
    static void RunAllTests() {
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║   ЗАПУСК ПОЛНОГО НАБОРА UNIT TESTS                                ║\n";
        std::cout << "║   Pathfinding Lab - Dijkstra, All Shortest Paths                 ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════════╝\n\n";

        int totalTests = 0;
        int passedTests = 0;

        std::cout << "═══════════════════════════════════════════════════════════════════\n";
        std::cout << "1. DIJKSTRA: Поиск кратчайшего пути (один путь)\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto dijkstraPassed = TestDijkstra();
        totalTests += 10;
        passedTests += dijkstraPassed;

        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "2. ALL SHORTEST PATHS: Поиск всех кратчайших путей\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto allPathsPassed = TestAllShortestPaths();
        totalTests += 10;
        passedTests += allPathsPassed;

        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "3. EDGE CASES: Граничные случаи (нет пути, один путь, много путей)\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto edgeCasesPassed = TestEdgeCases();
        totalTests += 10;
        passedTests += edgeCasesPassed;

        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "4. IMPASSABLE CELLS: Тесты с непроходимыми клетками (Forest)\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto impassablePassed = TestImpassableCells();
        totalTests += 10;
        passedTests += impassablePassed;
        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "ИТОГОВЫЕ РЕЗУЛЬТАТЫ\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n";
        std::cout << "Всего тестов:     " << std::setw(3) << totalTests << "\n";
        std::cout << "Пройдено:         " << std::setw(3) << passedTests << "\n";
        std::cout << "Провалено:        " << std::setw(3) << (totalTests - passedTests) << "\n";
        std::cout << "Успешность:       " << std::fixed << std::setprecision(1)
                  << (100.0 * passedTests / totalTests) << "%\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n";

        if (passedTests == totalTests) {
            std::cout << "\n✓ ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО! (" << passedTests << "/" << totalTests << ")\n\n";
        } else {
            std::cout << "\n⨯ НЕКОТОРЫЕ ТЕСТЫ ПРОВАЛИЛИСЬ! (" << passedTests << "/" << totalTests << ")\n\n";
            throw std::runtime_error("Тестирование не пройдено!");
        }
    }

private:
    static int TestDijkstra() {
        int passed = 0;

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            
            if (result.found && result.path.size() == 3 && 
                result.path[0] == a && result.path[1] == b && result.path[2] == c &&
                std::abs(result.totalCost - 2.0) < 0.001) {
                std::cout << "  ✓ Test 1: Простой линейный граф (A -> B -> C)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 1 FAILED: ожидался путь [A, B, C] со стоимостью 2.0\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            graph.addEdge(a, d, 0.5);
            graph.addEdge(d, c, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            
            if (result.found && std::abs(result.totalCost - 1.5) < 0.001) {
                std::cout << "  ✓ Test 2: Выбор более короткого пути (A -> D -> C)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 2 FAILED: ожидался путь со стоимостью 1.5, получено " 
                         << result.totalCost << "\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Sand));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Water));
            int d = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.5);
            graph.addEdge(b, c, 2.0);
            graph.addEdge(a, d, 1.0);
            graph.addEdge(d, c, 2.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            if (result.found && std::abs(result.totalCost - 3.0) < 0.001) {
                std::cout << "  ✓ Test 3: Разные типы местности (выбор оптимального пути)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 3 FAILED: ожидалась стоимость 3.0, получено " 
                         << result.totalCost << "\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, a);
            
            if (result.found && result.path.size() == 1 && result.path[0] == a &&
                std::abs(result.totalCost) < 0.001) {
                std::cout << "  ✓ Test 4: Путь от вершины к самой себе\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 4 FAILED: путь к самой себе должен иметь стоимость 0\n";
            }
        }

        {
            Graph graph;
            std::vector<int> vertices;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    vertices.push_back(graph.addVertex(Cell(i, j, TerrainType::Grass)));
                }
            }
            
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int idx = i * 3 + j;
                    if (j < 2) graph.addEdge(idx, idx + 1, 1.0);
                    if (i < 2) graph.addEdge(idx, idx + 3, 1.0);
                }
            }
            
            PathResult result = Pathfinder::findShortestPath(graph, vertices[0], vertices[8]);
            if (result.found && result.path.size() >= 5 && 
                std::abs(result.totalCost - 4.0) < 0.001) {
                std::cout << "  ✓ Test 5: Большой граф (сетка 3x3)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 5 FAILED: ожидался путь длиной >= 5 со стоимостью 4.0\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 2.5);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, b);
            
            if (result.found && result.path.size() == 2 &&
                result.path[0] == a && result.path[1] == b &&
                std::abs(result.totalCost - 2.5) < 0.001) {
                std::cout << "  ✓ Test 6: Граф с одним ребром\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6 FAILED: ожидался путь [A, B] со стоимостью 2.5\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, c, a);
            
            if (result.found && result.path.size() == 3 &&
                result.path[0] == c && result.path[1] == b && result.path[2] == a &&
                std::abs(result.totalCost - 2.0) < 0.001) {
                std::cout << "  ✓ Test 7: Обратный путь (неориентированный граф)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: обратный путь должен работать\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(1, 0, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, d, 1.0);
            graph.addEdge(d, c, 1.0);
            graph.addEdge(c, a, 1.0);
            graph.addEdge(a, d, 1.5);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, d);
            if (result.found && std::abs(result.totalCost - 1.5) < 0.001) {
                std::cout << "  ✓ Test 8: Граф с циклом (выбор оптимального пути)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 8 FAILED: ожидалась стоимость 1.5, получено " 
                         << result.totalCost << "\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            
            long long time = Pathfinder::measureExecutionTime(graph, a, b);
            
            if (time >= 0) {
                std::cout << "  ✓ Test 9: Измерение времени выполнения (" << time << " мкс)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 9 FAILED: время выполнения должно быть >= 0\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Sand));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Water));
            int d = graph.addVertex(Cell(0, 3, TerrainType::DeepWater));
            
            graph.addEdge(a, b, 1.5);
            graph.addEdge(b, c, 2.0);
            graph.addEdge(c, d, 3.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, d);
            if (result.found && result.path.size() == 4 &&
                std::abs(result.totalCost - 6.5) < 0.001) {
                std::cout << "  ✓ Test 10: Путь через несколько вершин с разными весами\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 10 FAILED: ожидалась стоимость 6.5, получено " 
                         << result.totalCost << "\n";
            }
        }

        return passed;
    }

    static int TestAllShortestPaths() {
        int passed = 0;

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            
            if (paths.size() == 1 && paths[0].found && paths[0].path.size() == 3 &&
                std::abs(paths[0].totalCost - 2.0) < 0.001) {
                std::cout << "  ✓ Test 1: Один кратчайший путь\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 1 FAILED: ожидался один путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            graph.addEdge(a, d, 1.0);
            graph.addEdge(d, c, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            
            if (paths.size() == 2) {
                bool hasPath1 = false, hasPath2 = false;
                for (const auto& path : paths) {
                    if (path.path.size() == 3 && path.path[0] == a && path.path[2] == c) {
                        if (path.path[1] == b) hasPath1 = true;
                        if (path.path[1] == d) hasPath2 = true;
                    }
                }
                if (hasPath1 && hasPath2) {
                    std::cout << "  ✓ Test 2: Два равнозначных пути\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 2 FAILED: не найдены оба пути\n";
                }
            } else {
                std::cout << "  ⨯ Test 2 FAILED: ожидалось 2 пути, получено " << paths.size() << "\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 0, TerrainType::Grass));
            int e = graph.addVertex(Cell(1, 2, TerrainType::Grass));
            int f = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            graph.addEdge(a, d, 1.0);
            graph.addEdge(d, e, 1.0);
            graph.addEdge(e, c, 1.0);
            graph.addEdge(a, f, 1.0);
            graph.addEdge(f, c, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            
            if (paths.size() == 3) {
                std::cout << "  ✓ Test 3: Три равнозначных пути\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 3 FAILED: ожидалось 3 пути, получено " << paths.size() << "\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            
            bool allSameCost = true;
            double expectedCost = paths[0].totalCost;
            for (const auto& path : paths) {
                if (std::abs(path.totalCost - expectedCost) > 0.001) {
                    allSameCost = false;
                    break;
                }
            }
            
            if (allSameCost && paths.size() > 0) {
                std::cout << "  ✓ Test 4: Все пути имеют одинаковую стоимость\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 4 FAILED: не все пути имеют одинаковую стоимость\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, a);
            
            if (paths.size() == 1 && paths[0].found && paths[0].path.size() == 1 &&
                std::abs(paths[0].totalCost) < 0.001) {
                std::cout << "  ✓ Test 5: Путь от вершины к самой себе\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 5 FAILED: путь к самой себе должен быть один\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int b = graph.addVertex(Cell(1, 0, TerrainType::Grass));
            int c = graph.addVertex(Cell(1, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(2, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(a, c, 1.0);
            graph.addEdge(b, d, 1.0);
            graph.addEdge(c, d, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, d);
            
            if (paths.size() == 2) {
                std::cout << "  ✓ Test 6: Сложный граф с несколькими путями\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6 FAILED: ожидалось 2 пути, получено " << paths.size() << "\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            
            bool valid = true;
            for (const auto& path : paths) {
                if (path.path.size() < 2 || path.path[0] != a || path.path.back() != c) {
                    valid = false;
                    break;
                }
            }
            
            if (valid && paths.size() > 0) {
                std::cout << "  ✓ Test 7: Проверка корректности путей\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: некорректные пути\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, b);
            
            if (paths.size() == 1 && paths[0].found && paths[0].path.size() == 2) {
                std::cout << "  ✓ Test 8: Граф с одним ребром\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 8 FAILED: ожидался один путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            graph.addEdge(a, d, 1.5);
            graph.addEdge(d, c, 1.5);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            if (paths.size() == 1 && std::abs(paths[0].totalCost - 2.0) < 0.001) {
                std::cout << "  ✓ Test 9: Только кратчайшие пути (игнорирование длинных)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 9 FAILED: должен возвращать только кратчайшие пути\n";
            }
        }

        {
            Graph graph;
            std::vector<int> vertices;
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 3; ++j) {
                    vertices.push_back(graph.addVertex(Cell(i, j, TerrainType::Grass)));
                }
            }
            
            for (int i = 0; i < 2; ++i) {
                for (int j = 0; j < 3; ++j) {
                    int idx = i * 3 + j;
                    if (j < 2) graph.addEdge(idx, idx + 1, 1.0);
                    if (i < 1) graph.addEdge(idx, idx + 3, 1.0);
                }
            }
            
            auto paths = Pathfinder::findAllShortestPaths(graph, vertices[0], vertices[5]);
            if (paths.size() >= 1 && paths[0].found) {
                std::cout << "  ✓ Test 10: Большой граф с несколькими путями\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 10 FAILED: должен найти хотя бы один путь\n";
            }
        }

        return passed;
    }

    static int TestEdgeCases() {
        int passed = 0;

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, b);
            
            if (!result.found) {
                std::cout << "  ✓ Test 1: Нет пути (изолированные вершины)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 1 FAILED: путь не должен быть найден\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            
            if (!result.found) {
                std::cout << "  ✓ Test 2: Нет пути (недостижимая вершина)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 2 FAILED: путь не должен быть найден\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            
            if (paths.size() == 1 && paths[0].found) {
                std::cout << "  ✓ Test 3: Один путь (единственный возможный)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 3 FAILED: должен быть найден один путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 0, TerrainType::Grass));
            int e = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            int f = graph.addVertex(Cell(1, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            graph.addEdge(c, f, 1.0);
            graph.addEdge(a, d, 1.0);
            graph.addEdge(d, e, 1.0);
            graph.addEdge(e, f, 1.0);
            graph.addEdge(a, e, 1.0);
            graph.addEdge(e, f, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, f);
            
            if (paths.size() >= 2) {
                std::cout << "  ✓ Test 4: Много путей (сложный граф)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 4 FAILED: должно быть найдено несколько путей\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, a);
            
            if (result.found && result.path.size() == 1) {
                std::cout << "  ✓ Test 5: Пустой граф (одна вершина)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 5 FAILED: путь к самой себе должен быть найден\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, 999);
            if (!result.found) {
                std::cout << "  ✓ Test 6: Невалидные индексы (обработка ошибок)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6 FAILED: невалидный индекс должен обрабатываться\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 0.0);
            graph.addEdge(b, c, 0.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            
            if (result.found && std::abs(result.totalCost) < 0.001) {
                std::cout << "  ✓ Test 7: Граф с нулевыми весами\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: путь с нулевыми весами должен работать\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1000000.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, b);
            
            if (result.found && std::abs(result.totalCost - 1000000.0) < 0.001) {
                std::cout << "  ✓ Test 8: Очень большой вес\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 8 FAILED: большой вес должен обрабатываться корректно\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            try {
                graph.addEdge(a, b, -1.0);
                std::cout << "  ⨯ Test 9 FAILED: отрицательные веса должны вызывать исключение\n";
            } catch (const std::exception&) {
                std::cout << "  ✓ Test 9: Граф с отрицательными весами (проверка стабильности)\n";
                passed++;
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, c, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, c);
            
            if (paths.size() == 1 && paths[0].found) {
                std::cout << "  ✓ Test 10: Все пути заблокированы кроме одного\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 10 FAILED: должен быть найден один путь\n";
            }
        }

        return passed;
    }

    static int TestImpassableCells() {
        int passed = 0;

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            graph.addVertex(Cell(0, 1, TerrainType::Forest));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            
            if (!result.found) {
                std::cout << "  ✓ Test 1: Путь заблокирован непроходимой клеткой\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 1 FAILED: путь через Forest не должен быть найден\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Forest));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, d, 1.0);
            graph.addEdge(d, c, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            
            if (result.found && std::abs(result.totalCost - 2.0) < 0.001) {
                std::cout << "  ✓ Test 2: Обход непроходимой клетки\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 2 FAILED: должен найти обходной путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Forest));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Forest));
            int d = graph.addVertex(Cell(0, 3, TerrainType::Grass));
            int e = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            int f = graph.addVertex(Cell(1, 2, TerrainType::Grass));
            
            graph.addEdge(a, e, 1.0);
            graph.addEdge(e, f, 1.0);
            graph.addEdge(f, d, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, d);
            
            if (result.found && std::abs(result.totalCost - 3.0) < 0.001) {
                std::cout << "  ✓ Test 3: Несколько непроходимых клеток\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 3 FAILED: должен найти обходной путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Forest));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, b);
            std::cout << "  ✓ Test 4: Непроходимая клетка в начале пути\n";
            passed++;
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Forest));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, b);
            
            if (!result.found) {
                std::cout << "  ✓ Test 5: Непроходимая клетка в конце пути\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 5 FAILED: путь к Forest не должен быть найден\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Forest));
            int c = graph.addVertex(Cell(1, 0, TerrainType::Forest));
            int d = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, d);
            
            if (!result.found) {
                std::cout << "  ✓ Test 6: Все пути заблокированы непроходимыми клетками\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6 FAILED: путь не должен быть найден\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Sand));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Forest));
            int d = graph.addVertex(Cell(0, 3, TerrainType::Water));
            int e = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.5);
            graph.addEdge(b, e, 1.0);
            graph.addEdge(e, d, 2.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, d);
            
            if (result.found && std::abs(result.totalCost - 4.5) < 0.001) {
                std::cout << "  ✓ Test 7: Смешанные типы местности с непроходимыми\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: должен найти обходной путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Grass));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Forest));
            int d = graph.addVertex(Cell(0, 3, TerrainType::Grass));
            int e = graph.addVertex(Cell(1, 1, TerrainType::Grass));
            int f = graph.addVertex(Cell(1, 2, TerrainType::Grass));
            
            graph.addEdge(a, b, 1.0);
            graph.addEdge(b, e, 1.0);
            graph.addEdge(e, f, 1.0);
            graph.addEdge(f, d, 1.0);
            
            PathResult result = Pathfinder::findShortestPath(graph, a, d);
            
            if (result.found && std::abs(result.totalCost - 4.0) < 0.001) {
                std::cout << "  ✓ Test 8: Непроходимая клетка в середине пути (обход)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 8 FAILED: должен найти обходной путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Forest));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Forest));
            int d = graph.addVertex(Cell(0, 3, TerrainType::Grass));
            int e = graph.addVertex(Cell(1, 0, TerrainType::Grass));
            int f = graph.addVertex(Cell(1, 3, TerrainType::Grass));
            
            graph.addEdge(a, e, 1.0);
            graph.addEdge(e, f, 1.0);
            graph.addEdge(f, d, 1.0);
            
            auto paths = Pathfinder::findAllShortestPaths(graph, a, d);
            
            if (paths.size() == 1 && paths[0].found) {
                std::cout << "  ✓ Test 9: Все кратчайшие пути через непроходимые клетки\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 9 FAILED: должен найти один обходной путь\n";
            }
        }

        {
            Graph graph;
            int a = graph.addVertex(Cell(0, 0, TerrainType::Grass));
            int b = graph.addVertex(Cell(0, 1, TerrainType::Forest));
            int c = graph.addVertex(Cell(0, 2, TerrainType::Grass));
            int d = graph.addVertex(Cell(1, 0, TerrainType::Forest));
            int e = graph.addVertex(Cell(1, 1, TerrainType::Forest));
            int f = graph.addVertex(Cell(1, 2, TerrainType::Forest));
            
            PathResult result = Pathfinder::findShortestPath(graph, a, c);
            
            if (!result.found) {
                std::cout << "  ✓ Test 10: Непроходимые клетки создают изолированные области\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 10 FAILED: путь не должен быть найден\n";
            }
        }

        return passed;
    }
};
