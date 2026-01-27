#pragma once
#include <set>
#include <map>
#include <string>
#include <queue>
#include <sstream>

class NFAtoDFAConverter {
public:
    using StateSet = std::set<int>;
    
    struct NFA {
        std::set<int> states;
        std::set<char> alphabet;
        std::map<std::pair<int, char>, std::set<int>> transitions;
        int initialState;
        std::set<int> acceptingStates;
        std::map<int, std::set<int>> epsilonTransitions;
    };
    
    struct DFA {
        std::set<StateSet> states;
        std::set<char> alphabet;
        std::map<std::pair<StateSet, char>, StateSet> transitions;
        StateSet initialState;
        std::set<StateSet> acceptingStates;
    };
    
    static StateSet EpsilonClosure(int state, const NFA& nfa) {
        StateSet closure;
        std::queue<int> toProcess;
        
        closure.insert(state);
        toProcess.push(state);
        
        while (!toProcess.empty()) {
            int current = toProcess.front();
            toProcess.pop();
            
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
    
    static DFA Convert(const NFA& nfa) {
        DFA dfa;
        dfa.alphabet = nfa.alphabet;
        
        StateSet initialDFAState = EpsilonClosure(nfa.initialState, nfa);
        dfa.initialState = initialDFAState;
        
        std::queue<StateSet> toProcess;
        toProcess.push(initialDFAState);
        dfa.states.insert(initialDFAState);
        
        while (!toProcess.empty()) {
            StateSet currentDFAState = toProcess.front();
            toProcess.pop();
            
            for (int nfaState : currentDFAState) {
                if (nfa.acceptingStates.count(nfaState) > 0) {
                    dfa.acceptingStates.insert(currentDFAState);
                    break;
                }
            }
            
            for (char symbol : nfa.alphabet) {
                StateSet afterMove = Move(currentDFAState, symbol, nfa);
                
                if (!afterMove.empty()) {
                    StateSet nextDFAState = EpsilonClosureSet(afterMove, nfa);
                    
                    auto transitionKey = std::make_pair(currentDFAState, symbol);
                    dfa.transitions[transitionKey] = nextDFAState;
                    
                    if (dfa.states.find(nextDFAState) == dfa.states.end()) {
                        dfa.states.insert(nextDFAState);
                        toProcess.push(nextDFAState);
                    }
                }
            }
        }
        
        return dfa;
    }
    
    static bool AcceptsString(const DFA& dfa, const std::string& str) {
        StateSet currentState = dfa.initialState;
        
        for (char symbol : str) {
            auto transitionKey = std::make_pair(currentState, symbol);
            auto it = dfa.transitions.find(transitionKey);
            
            if (it == dfa.transitions.end()) {
                return false;
            }
            
            currentState = it->second;
        }
        
        return dfa.acceptingStates.count(currentState) > 0;
    }
    
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
