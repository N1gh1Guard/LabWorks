#pragma once
#include <set>
#include <map>
#include <vector>
#include <string>
#include <queue>
#include <sstream>

class NFAtoDFAConverter {
public:
    // Тип для представления набора NFA состояний
    using StateSet = std::set<int>;
    
    // Определение NFA (недетерминированный конечный автомат)
    struct NFA {
        std::set<int> states;
        std::set<char> alphabet;
        // Переходы: (текущее состояние, символ) -> набор следующих состояний
        std::map<std::pair<int, char>, std::set<int>> transitions;
        int initialState;
        std::set<int> acceptingStates;
        
        // Epsilon переходы: состояние -> набор достижимых через eps состояний
        std::map<int, std::set<int>> epsilonTransitions;
    };
    
    // Определение DFA (детерминированный конечный автомат)
    struct DFA {
        std::set<StateSet> states;
        std::set<char> alphabet;
        // Переходы: (набор NFA состояний, символ) -> новый набор NFA состояний
        std::map<std::pair<StateSet, char>, StateSet> transitions;
        StateSet initialState;
        std::set<StateSet> acceptingStates;
    };
    
    // Вычисление epsilon-замыкания для одного состояния
    static StateSet EpsilonClosure(int state, const NFA& nfa) {
        StateSet closure;
        std::queue<int> toProcess;
        
        closure.insert(state);
        toProcess.push(state);
        
        while (!toProcess.empty()) {
            int current = toProcess.front();
            toProcess.pop();
            
            // Проверяем epsilon переходы из текущего состояния
            auto it = nfa.epsilonTransitions.find(current);
            if (it != nfa.epsilonTransitions.end()) {
                for (int nextState : it->second) {
                    if (closure.find(nextState) == closure.end()) {
                        closure.insert(nextState);
                        toProcess.push(nextState);
                    }
                }
            }
        }
        
        return closure;
    }
    
    // Вычисление epsilon-замыкания для набора состояний
    static StateSet EpsilonClosureSet(const StateSet& states, const NFA& nfa) {
        StateSet closure;
        
        for (int state : states) {
            StateSet stateClosure = EpsilonClosure(state, nfa);
            for (int s : stateClosure) {
                closure.insert(s);
            }
        }
        
        return closure;
    }
    
    // Вычисление переходов для набора NFA состояний по символу
    static StateSet Move(const StateSet& states, char symbol, const NFA& nfa) {
        StateSet result;
        
        for (int state : states) {
            auto key = std::make_pair(state, symbol);
            auto it = nfa.transitions.find(key);
            
            if (it != nfa.transitions.end()) {
                for (int nextState : it->second) {
                    result.insert(nextState);
                }
            }
        }
        
        return result;
    }
    
    // Основной метод конвертации NFA в DFA (Powerset Construction)
    static DFA Convert(const NFA& nfa) {
        DFA dfa;
        dfa.alphabet = nfa.alphabet;
        
        // Начальное состояние DFA — epsilon-замыкание начального состояния NFA
        StateSet initialDFAState = EpsilonClosure(nfa.initialState, nfa);
        dfa.initialState = initialDFAState;
        
        std::queue<StateSet> toProcess;
        toProcess.push(initialDFAState);
        dfa.states.insert(initialDFAState);
        
        // BFS по всем достижимым наборам состояний
        while (!toProcess.empty()) {
            StateSet currentDFAState = toProcess.front();
            toProcess.pop();
            
            // Проверяем, является ли это состояние принимающим
            // (содержит ли оно хотя бы одно принимающее NFA состояние)
            for (int nfaState : currentDFAState) {
                if (nfa.acceptingStates.count(nfaState) > 0) {
                    dfa.acceptingStates.insert(currentDFAState);
                    break;
                }
            }
            
            // Для каждого символа алфавита
            for (char symbol : nfa.alphabet) {
                // Вычисляем Move(currentDFAState, symbol)
                StateSet afterMove = Move(currentDFAState, symbol, nfa);
                
                // Если есть переходы
                if (!afterMove.empty()) {
                    // Берем epsilon-замыкание результата
                    StateSet nextDFAState = EpsilonClosureSet(afterMove, nfa);
                    
                    // Добавляем переход в DFA
                    auto transitionKey = std::make_pair(currentDFAState, symbol);
                    dfa.transitions[transitionKey] = nextDFAState;
                    
                    // Если новое состояние еще не обработано, добавляем в очередь
                    if (dfa.states.find(nextDFAState) == dfa.states.end()) {
                        dfa.states.insert(nextDFAState);
                        toProcess.push(nextDFAState);
                    }
                }
            }
        }
        
        return dfa;
    }
    
    // Проверка, принимает ли DFA строку
    static bool AcceptsString(const DFA& dfa, const std::string& str) {
        StateSet currentState = dfa.initialState;
        
        for (char symbol : str) {
            auto transitionKey = std::make_pair(currentState, symbol);
            auto it = dfa.transitions.find(transitionKey);
            
            if (it == dfa.transitions.end()) {
                // Нет переходов для этого символа
                return false;
            }
            
            currentState = it->second;
        }
        
        // Проверяем, в принимающем ли состоянии мы закончили
        return dfa.acceptingStates.count(currentState) > 0;
    }
    
    // Вспомогательная функция для преобразования StateSet в строку
    static std::string StateSetToString(const StateSet& states) {
        std::stringstream ss;
        ss << "{";
        
        bool first = true;
        for (int state : states) {
            if (!first) ss << ",";
            ss << state;
            first = false;
        }
        
        ss << "}";
        return ss.str();
    }
    
    // Визуализация DFA (для отладки и анализа)
    static std::string VisualizeDFA(const DFA& dfa) {
        std::stringstream ss;
        
        ss << "DFA States Count: " << dfa.states.size() << "\n";
        ss << "Accepting States Count: " << dfa.acceptingStates.size() << "\n";
        ss << "\nTransitions:\n";
        
        for (const auto& transition : dfa.transitions) {
            const StateSet& fromState = transition.first.first;
            char symbol = transition.first.second;
            const StateSet& toState = transition.second;
            
            ss << "  " << StateSetToString(fromState) 
               << " --" << symbol << "--> "
               << StateSetToString(toState) << "\n";
        }
        
        ss << "\nInitial State: " << StateSetToString(dfa.initialState) << "\n";
        ss << "Accepting States:\n";
        for (const auto& acceptState : dfa.acceptingStates) {
            ss << "  " << StateSetToString(acceptState) << "\n";
        }
        
        return ss.str();
    }
    
    // Получение информации о DFA
    struct DFAInfo {
        size_t statesCount;
        size_t transitionsCount;
        size_t acceptingStatesCount;
    };
    
    static DFAInfo GetDFAInfo(const DFA& dfa) {
        return {
            dfa.states.size(),
            dfa.transitions.size(),
            dfa.acceptingStates.size()
        };
    }
};
