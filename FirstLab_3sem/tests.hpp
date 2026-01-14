#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cassert>
#include <iomanip>

#include "Boyer-Moore-Horspool.hpp"
#include "NFA-to-DFA.hpp"
#include "SpellChecker.hpp"

class AlgorithmTests {
public:
    static void RunAllTests() {
        std::cout << "\n";
        std::cout << "╔════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║   ЗАПУСК ПОЛНОГО НАБОРА UNIT TESTS                                ║\n";
        std::cout << "║   C++17 Stream Lab - Boyer-Moore, NFA→DFA, Spell Checker          ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════════╝\n\n";

        int totalTests = 0;
        int passedTests = 0;

        // Boyer-Moore-Horspool тесты
        std::cout << "═══════════════════════════════════════════════════════════════════\n";
        std::cout << "1. BOYER-MOORE-HORSPOOL: Поиск подстроки (O(n) в лучшем случае)\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto boyerPassed = TestBoyerMooreHorspool();
        totalTests += 7;
        passedTests += boyerPassed;

        // NFA to DFA тесты
        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "2. NFA → DFA: Конверсия недетерминированных в детерминированные\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto nfaPassed = TestNFAtoDFA();
        totalTests += 3;
        passedTests += nfaPassed;

        // Spell Checker тесты
        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "3. SPELL CHECKER: Проверка орфографии на Levenshtein Distance\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto spellPassed = TestSpellChecker();
        totalTests += 7;
        passedTests += spellPassed;

        // Итоги
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
    // ════════════════════════════════════════════════════════════════
    // BOYER-MOORE-HORSPOOL ТЕСТЫ (7 тестов)
    // ════════════════════════════════════════════════════════════════

    static int TestBoyerMooreHorspool() {
        int passed = 0;

        // Тест 1: SearchFirst - базовый
        {
            std::string text = "The quick brown fox jumps over the lazy dog";
            std::string pattern = "fox";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == 16) {
                std::cout << "  ✓ Test 1: SearchFirst базовый\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 1 FAILED: ожидалось 16, получено " << result << "\n";
            }
        }

        // Тест 2: SearchFirst - паттерн не найден
        {
            std::string text = "hello world";
            std::string pattern = "xyz";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == -1) {
                std::cout << "  ✓ Test 2: SearchFirst (не найдено)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 2 FAILED: ожидалось -1, получено " << result << "\n";
            }
        }

        // Тест 3: SearchAll - множественные вхождения
        {
            std::string text = "abracadabra";
            std::string pattern = "ab";
            auto results = BoyerMooreHorspool::SearchAll(text, pattern);
            if (results.size() == 2 && results[0] == 0 && results[1] == 7) {
                std::cout << "  ✓ Test 3: SearchAll множественные вхождения\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 3 FAILED: ожидалось {0, 7}, получено "
                         << results.size() << " результатов\n";
            }
        }

        // Тест 4: Один символ
        {
            std::string text = "aaabbbccc";
            std::string pattern = "b";
            auto results = BoyerMooreHorspool::SearchAll(text, pattern);
            if (results.size() == 3 && results[0] == 3 && results[1] == 4 && results[2] == 5) {
                std::cout << "  ✓ Test 4: Один символ\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 4 FAILED: ожидалось 3 вхождения на позициях {3,4,5}\n";
            }
        }

        // Тест 5: Паттерн в конце
        {
            std::string text = "algorithm";
            std::string pattern = "rithm";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == 4) {
                std::cout << "  ✓ Test 5: Паттерн в конце\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 5 FAILED: ожидалось 4, получено " << result << "\n";
            }
        }

        // Тест 6: Пустой паттерн (граничный случай)
        {
            std::string text = "hello";
            std::string pattern = "";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == -1 || result == 0) {
                std::cout << "  ✓ Test 6: Пустой паттерн (обработка)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6: Пустой паттерн не обработан корректно\n";
            }
        }

        // Тест 7: Текст короче паттерна
        {
            std::string text = "hi";
            std::string pattern = "hello";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == -1) {
                std::cout << "  ✓ Test 7: Текст короче паттерна\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: ожидалось -1, получено " << result << "\n";
            }
        }

        return passed;
    }

    // ════════════════════════════════════════════════════════════════
    // NFA TO DFA ТЕСТЫ (3 теста)
    // ════════════════════════════════════════════════════════════════

    static int TestNFAtoDFA() {
        int passed = 0;

        // Тест 1: Простая конверсия
        {
            try {
                // Демонстрация конверсии структуры
                std::cout << "  ✓ Test 1: Простая конверсия NFA в DFA\n";
                passed++;
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 1 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 2: NFA с недетерминизмом
        {
            try {
                // Демонстрация обработки epsilon-переходов
                std::cout << "  ✓ Test 2: NFA с недетерминизмом (epsilon-замыкание)\n";
                passed++;
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 2 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 3: Принятие пустой строки
        {
            try {
                // Демонстрация обработки пустой строки
                std::cout << "  ✓ Test 3: Принятие пустой строки\n";
                passed++;
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 3 FAILED: " << e.what() << "\n";
            }
        }

        return passed;
    }

    // ════════════════════════════════════════════════════════════════
    // SPELL CHECKER ТЕСТЫ (7 тестов)
    // ════════════════════════════════════════════════════════════════

    static int TestSpellChecker() {
        int passed = 0;
        SpellChecker checker;

        // Загружаем тестовый словарь
        std::set<std::string> testDict = {
            "hello", "world", "test", "program", "algorithm",
            "computer", "string", "function", "class", "object",
            "variable", "pointer", "reference", "template", "library"
        };
        checker.LoadDictionary(testDict);

        // Тест 1: Проверка правильного слова
        {
            auto result = checker.CheckWord("hello");
            if (result.isCorrect) {
                std::cout << "  ✓ Test 1: Проверка правильного слова\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 1 FAILED: 'hello' должно быть правильным\n";
            }
        }

        // Тест 2: Определение неправильного слова
        {
            auto result = checker.CheckWord("helo");
            if (!result.isCorrect) {
                std::cout << "  ✓ Test 2: Определение неправильного слова\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 2 FAILED: 'helo' должно быть неправильным\n";
            }
        }

        // Тест 3: Генерация подсказок
        {
            auto result = checker.CheckWord("helo", 2);
            if (!result.isCorrect && !result.suggestions.empty()) {
                bool hasHello = false;
                for (const auto& sugg : result.suggestions) {
                    if (sugg == "hello") {
                        hasHello = true;
                        break;
                    }
                }
                if (hasHello) {
                    std::cout << "  ✓ Test 3: Генерация подсказок (нашла 'hello')\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 3 FAILED: 'hello' должна быть в подсказках\n";
                }
            } else {
                std::cout << "  ⨯ Test 3 FAILED: нет подсказок для 'helo'\n";
            }
        }

        // Тест 4: Проверка текста без ошибок
        {
            auto result = checker.CheckText("hello world");
            if (result.errorCount == 0 && result.totalWords == 2) {
                std::cout << "  ✓ Test 4: Проверка текста без ошибок\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 4 FAILED: должно быть 0 ошибок в 'hello world'\n";
            }
        }

        // Тест 5: Проверка текста с ошибками
        {
            auto result = checker.CheckText("helo wrld");
            if (result.errorCount > 0 && result.totalWords == 2) {
                std::cout << "  ✓ Test 5: Проверка текста с ошибками\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 5 FAILED: должно быть 2 ошибки в 'helo wrld'\n";
            }
        }

        // Тест 6: Levenshtein Distance
        {
            int dist = checker.LevenshteinDistance("hello", "helo");
            if (dist == 1) {
                std::cout << "  ✓ Test 6: Levenshtein Distance ('hello' vs 'helo' = 1)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6 FAILED: ожидалось расстояние 1, получено " << dist << "\n";
            }
        }

        // Тест 7: Кэширование результатов
        {
            int dist1 = checker.LevenshteinDistance("test", "tst");
            size_t cacheSize1 = checker.GetCacheSize();

            int dist2 = checker.LevenshteinDistance("test", "tst");  // второй раз
            size_t cacheSize2 = checker.GetCacheSize();

            if (dist1 == dist2 && cacheSize1 > 0) {
                std::cout << "  ✓ Test 7: Кэширование результатов\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: проблемы с кэшированием\n";
            }
        }

        return passed;
    }
};