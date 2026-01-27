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
        totalTests += 10;  // Увеличено с 7 до 10
        passedTests += boyerPassed;

        // NFA to DFA тесты
        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "2. NFA → DFA: Конверсия недетерминированных в детерминированные\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto nfaPassed = TestNFAtoDFA();
        totalTests += 7;  // Увеличено с 3 до 7
        passedTests += nfaPassed;

        // Spell Checker тесты
        std::cout << "\n═══════════════════════════════════════════════════════════════════\n";
        std::cout << "3. SPELL CHECKER: Проверка орфографии на Levenshtein Distance\n";
        std::cout << "═══════════════════════════════════════════════════════════════════\n\n";

        auto spellPassed = TestSpellChecker();
        totalTests += 10;  // Увеличено с 7 до 10
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
    // BOYER-MOORE-HORSPOOL ТЕСТЫ (10 тестов)
    // ════════════════════════════════════════════════════════════════

    static int TestBoyerMooreHorspool() {
        int passed = 0;

        // Тест 1: SearchFirst - базовый поиск
        {
            std::string text = "The quick brown fox jumps over the lazy dog";
            std::string pattern = "fox";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == 16) {
                std::cout << "  ✓ Test 1: SearchFirst базовый поиск\n";
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
                std::cout << "  ✓ Test 2: SearchFirst (паттерн не найден)\n";
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

        // Тест 4: SearchAll - один символ
        {
            std::string text = "aaabbbccc";
            std::string pattern = "b";
            auto results = BoyerMooreHorspool::SearchAll(text, pattern);
            if (results.size() == 3 && results[0] == 3 && results[1] == 4 && results[2] == 5) {
                std::cout << "  ✓ Test 4: SearchAll один символ\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 4 FAILED: ожидалось 3 вхождения на позициях {3,4,5}\n";
            }
        }

        // Тест 5: Паттерн в конце текста
        {
            std::string text = "algorithm";
            std::string pattern = "rithm";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == 4) {
                std::cout << "  ✓ Test 5: Паттерн в конце текста\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 5 FAILED: ожидалось 4, получено " << result << "\n";
            }
        }

        // Тест 6: Паттерн в начале текста
        {
            std::string text = "algorithm";
            std::string pattern = "algo";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == 0) {
                std::cout << "  ✓ Test 6: Паттерн в начале текста\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6 FAILED: ожидалось 0, получено " << result << "\n";
            }
        }

        // Тест 7: Паттерн равен тексту
        {
            std::string text = "hello";
            std::string pattern = "hello";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == 0) {
                std::cout << "  ✓ Test 7: Паттерн равен тексту\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: ожидалось 0, получено " << result << "\n";
            }
        }

        // Тест 8: Пустой паттерн (граничный случай)
        {
            std::string text = "hello";
            std::string pattern = "";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == -1) {
                std::cout << "  ✓ Test 8: Пустой паттерн (обработка)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 8 FAILED: пустой паттерн должен возвращать -1, получено " << result << "\n";
            }
        }

        // Тест 9: Текст короче паттерна
        {
            std::string text = "hi";
            std::string pattern = "hello";
            int result = BoyerMooreHorspool::SearchFirst(text, pattern);
            if (result == -1) {
                std::cout << "  ✓ Test 9: Текст короче паттерна\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 9 FAILED: ожидалось -1, получено " << result << "\n";
            }
        }

        // Тест 10: SearchResult структура
        {
            std::string text = "programming";
            std::string pattern = "gram";
            auto result = BoyerMooreHorspool::Search(text, pattern);
            if (result.isFound && result.position == 3 && result.foundPattern == "gram") {
                std::cout << "  ✓ Test 10: SearchResult структура\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 10 FAILED: некорректный SearchResult\n";
            }
        }

        return passed;
    }

    // ════════════════════════════════════════════════════════════════
    // NFA TO DFA ТЕСТЫ (7 тестов)
    // ════════════════════════════════════════════════════════════════

    static int TestNFAtoDFA() {
        int passed = 0;

        // Тест 1: Простая конверсия (a|b)
        {
            try {
                NFAtoDFAConverter::NFA nfa;
                nfa.states = {0, 1, 2, 3};
                nfa.alphabet = {'a', 'b'};
                nfa.initialState = 0;
                nfa.acceptingStates = {3};
                nfa.epsilonTransitions[0] = {1, 2};
                nfa.transitions[{1, 'a'}] = {3};
                nfa.transitions[{2, 'b'}] = {3};
                
                NFAtoDFAConverter::DFA dfa = NFAtoDFAConverter::Convert(nfa);
                
                if (dfa.states.size() > 0 && dfa.acceptingStates.size() > 0) {
                    std::cout << "  ✓ Test 1: Простая конверсия NFA в DFA (a|b)\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 1 FAILED: DFA не создан корректно\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 1 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 2: NFA с epsilon-переходами (ab*)
        {
            try {
                NFAtoDFAConverter::NFA nfa;
                nfa.states = {0, 1, 2};
                nfa.alphabet = {'a', 'b'};
                nfa.initialState = 0;
                nfa.acceptingStates = {2};
                nfa.transitions[{0, 'a'}] = {1};
                nfa.transitions[{1, 'b'}] = {1};
                nfa.epsilonTransitions[1] = {2};
                
                NFAtoDFAConverter::DFA dfa = NFAtoDFAConverter::Convert(nfa);
                auto info = NFAtoDFAConverter::GetDFAInfo(dfa);
                
                if (info.statesCount > 0 && info.transitionsCount > 0) {
                    std::cout << "  ✓ Test 2: NFA с epsilon-переходами (ab*)\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 2 FAILED: DFA не создан корректно\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 2 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 3: Проверка принятия строки (a|b)
        {
            try {
                NFAtoDFAConverter::NFA nfa;
                nfa.states = {0, 1, 2, 3};
                nfa.alphabet = {'a', 'b'};
                nfa.initialState = 0;
                nfa.acceptingStates = {3};
                nfa.epsilonTransitions[0] = {1, 2};
                nfa.transitions[{1, 'a'}] = {3};
                nfa.transitions[{2, 'b'}] = {3};
                
                NFAtoDFAConverter::DFA dfa = NFAtoDFAConverter::Convert(nfa);
                
                bool acceptsA = NFAtoDFAConverter::AcceptsString(dfa, "a");
                bool acceptsB = NFAtoDFAConverter::AcceptsString(dfa, "b");
                bool rejectsC = !NFAtoDFAConverter::AcceptsString(dfa, "c");
                
                if (acceptsA && acceptsB && rejectsC) {
                    std::cout << "  ✓ Test 3: Проверка принятия/отклонения строк (a|b)\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 3 FAILED: некорректное принятие/отклонение строк\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 3 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 4: Epsilon-замыкание для одного состояния
        {
            try {
                NFAtoDFAConverter::NFA nfa;
                nfa.states = {0, 1, 2};
                nfa.alphabet = {'a'};
                nfa.initialState = 0;
                nfa.acceptingStates = {2};
                nfa.epsilonTransitions[0] = {1};
                nfa.epsilonTransitions[1] = {2};
                nfa.transitions[{0, 'a'}] = {0};
                
                auto closure = NFAtoDFAConverter::EpsilonClosure(0, nfa);
                if (closure.size() >= 3) {  // Должно включать 0, 1, 2 через epsilon
                    std::cout << "  ✓ Test 4: Epsilon-замыкание для одного состояния\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 4 FAILED: некорректное epsilon-замыкание\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 4 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 5: Epsilon-замыкание для набора состояний
        {
            try {
                NFAtoDFAConverter::NFA nfa;
                nfa.states = {0, 1, 2, 3};
                nfa.alphabet = {'a'};
                nfa.initialState = 0;
                nfa.acceptingStates = {3};
                nfa.epsilonTransitions[0] = {1};
                nfa.epsilonTransitions[1] = {2};
                nfa.epsilonTransitions[2] = {3};
                
                NFAtoDFAConverter::StateSet states = {0, 1};
                auto closure = NFAtoDFAConverter::EpsilonClosureSet(states, nfa);
                if (closure.size() >= 3) {
                    std::cout << "  ✓ Test 5: Epsilon-замыкание для набора состояний\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 5 FAILED: некорректное epsilon-замыкание набора\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 5 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 6: Move функция (переходы по символу)
        {
            try {
                NFAtoDFAConverter::NFA nfa;
                nfa.states = {0, 1, 2};
                nfa.alphabet = {'a', 'b'};
                nfa.initialState = 0;
                nfa.acceptingStates = {2};
                nfa.transitions[{0, 'a'}] = {1};
                nfa.transitions[{1, 'b'}] = {2};
                
                NFAtoDFAConverter::StateSet states = {0};
                auto afterMove = NFAtoDFAConverter::Move(states, 'a', nfa);
                if (afterMove.size() == 1 && afterMove.count(1) > 0) {
                    std::cout << "  ✓ Test 6: Move функция (переходы по символу)\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 6 FAILED: некорректная функция Move\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 6 FAILED: " << e.what() << "\n";
            }
        }

        // Тест 7: Сложный NFA с циклом ((a|b)*)
        {
            try {
                NFAtoDFAConverter::NFA nfa;
                nfa.states = {0, 1, 2};
                nfa.alphabet = {'a', 'b'};
                nfa.initialState = 0;
                nfa.acceptingStates = {0, 2};
                nfa.epsilonTransitions[0] = {1};
                nfa.transitions[{1, 'a'}] = {2};
                nfa.transitions[{1, 'b'}] = {2};
                nfa.epsilonTransitions[2] = {1};
                
                NFAtoDFAConverter::DFA dfa = NFAtoDFAConverter::Convert(nfa);
                
                // Должен принимать пустую строку, "a", "b", "ab", "ba", и т.д.
                bool acceptsEmpty = NFAtoDFAConverter::AcceptsString(dfa, "");
                bool acceptsA = NFAtoDFAConverter::AcceptsString(dfa, "a");
                bool acceptsB = NFAtoDFAConverter::AcceptsString(dfa, "b");
                bool acceptsAB = NFAtoDFAConverter::AcceptsString(dfa, "ab");
                
                if (acceptsEmpty && acceptsA && acceptsB && acceptsAB) {
                    std::cout << "  ✓ Test 7: Сложный NFA с циклом ((a|b)*)\n";
                    passed++;
                } else {
                    std::cout << "  ⨯ Test 7 FAILED: некорректная обработка цикла\n";
                }
            } catch (const std::exception& e) {
                std::cout << "  ⨯ Test 7 FAILED: " << e.what() << "\n";
            }
        }

        return passed;
    }

    // ════════════════════════════════════════════════════════════════
    // SPELL CHECKER ТЕСТЫ (10 тестов)
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

        // Тест 6: Levenshtein Distance - базовый
        {
            int dist = checker.LevenshteinDistance("hello", "helo");
            if (dist == 1) {
                std::cout << "  ✓ Test 6: Levenshtein Distance базовый ('hello' vs 'helo' = 1)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 6 FAILED: ожидалось расстояние 1, получено " << dist << "\n";
            }
        }

        // Тест 7: Levenshtein Distance - пустые строки
        {
            int dist1 = checker.LevenshteinDistance("", "hello");
            int dist2 = checker.LevenshteinDistance("hello", "");
            if (dist1 == 5 && dist2 == 5) {
                std::cout << "  ✓ Test 7: Levenshtein Distance с пустыми строками\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 7 FAILED: некорректное расстояние с пустыми строками\n";
            }
        }

        // Тест 8: Кэширование результатов
        {
            int dist1 = checker.LevenshteinDistance("test", "tst");
            size_t cacheSize1 = checker.GetCacheSize();

            int dist2 = checker.LevenshteinDistance("test", "tst");  // второй раз
            size_t cacheSize2 = checker.GetCacheSize();

            // Второй вызов должен брать результат из кэша и не увеличивать его размер
            if (dist1 == dist2 && cacheSize1 > 0 && cacheSize2 == cacheSize1) {
                std::cout << "  ✓ Test 8: Кэширование результатов\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 8 FAILED: проблемы с кэшированием\n";
            }
        }

        // Тест 9: Добавление слова в словарь
        {
            size_t sizeBefore = checker.GetDictionarySize();
            checker.AddWord("newword");
            size_t sizeAfter = checker.GetDictionarySize();
            
            auto result = checker.CheckWord("newword");
            if (sizeAfter > sizeBefore && result.isCorrect) {
                std::cout << "  ✓ Test 9: Добавление слова в словарь\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 9 FAILED: слово не добавлено корректно\n";
            }
        }

        // Тест 10: Проверка регистра (case-insensitive)
        {
            auto result1 = checker.CheckWord("HELLO");
            auto result2 = checker.CheckWord("Hello");
            auto result3 = checker.CheckWord("hElLo");
            
            if (result1.isCorrect && result2.isCorrect && result3.isCorrect) {
                std::cout << "  ✓ Test 10: Проверка регистра (case-insensitive)\n";
                passed++;
            } else {
                std::cout << "  ⨯ Test 10 FAILED: некорректная обработка регистра\n";
            }
        }

        return passed;
    }
};
