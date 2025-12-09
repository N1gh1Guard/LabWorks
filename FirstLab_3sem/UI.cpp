#include "UI.h"
#include <iostream>
#include <vector>
#include <memory>
#include <sstream>

#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "Functions.h"
#include "Pair.h"
#include "Exception.h"

using namespace std;

class SequenceWrapper {
public:
    virtual ~SequenceWrapper() = default;
    virtual void Print() const = 0;
    virtual const char* TypeName() const = 0;
    virtual int GetLength() const = 0;
    virtual Sequence<int>* GetIntSequence() = 0;
    virtual Sequence<double>* GetDoubleSequence() = 0;
    virtual Sequence<Pair<int, int>>* GetPairIntSequence() = 0;
    virtual Sequence<Pair<double, double>>* GetPairDoubleSequence() = 0;
    virtual Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() = 0;
    virtual Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() = 0;
    
    virtual void AppendInt(int value) = 0;
    virtual void AppendDouble(double value) = 0;
    virtual void RemoveItemAt(int index) = 0;
};

template <typename T>
class TypedSequenceWrapper : public SequenceWrapper {
    Sequence<T>* sequence;
public:
    TypedSequenceWrapper(Sequence<T>* seq) : sequence(seq) {}
    ~TypedSequenceWrapper() { delete sequence; }

    void Print() const override {
        print(sequence);
    }

    const char* TypeName() const override {
        return sequence->TypeName();
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return nullptr;
    }

    Sequence<double>* GetDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, int>>* GetPairIntSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, double>>* GetPairDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() override {
        return nullptr;
    }
    
    void AppendInt(int value) override {
        throw MyException(5, "Cannot append int to this sequence type");
    }
    
    void AppendDouble(double value) override {
        throw MyException(5, "Cannot append double to this sequence type");
    }
    
    void RemoveItemAt(int index) override {
        throw MyException(5, "Cannot remove from this sequence type");
    }
};

class IntSequenceWrapper : public SequenceWrapper {
    Sequence<int>* sequence;
public:
    IntSequenceWrapper(Sequence<int>* seq) : sequence(seq) {}
    ~IntSequenceWrapper() { delete sequence; }

    void Print() const override {
        print(sequence);
    }

    const char* TypeName() const override {
        return sequence->TypeName();
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return sequence->Clone();
    }

    Sequence<double>* GetDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, int>>* GetPairIntSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, double>>* GetPairDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() override {
        return nullptr;
    }
    
    void AppendInt(int value) override {
        Sequence<int>* oldSeq = sequence;
        Sequence<int>* newSeq = sequence->Append(value);
        
        if (newSeq != oldSeq) {
            delete oldSeq;
            sequence = newSeq;
        }
    }
    
    void AppendDouble(double value) override {
        throw MyException(5, "Cannot append double to int sequence");
    }
    
    void RemoveItemAt(int index) override {
        Sequence<int>* oldSeq = sequence;
        Sequence<int>* newSeq = sequence->RemoveAt(index);
        
        if (newSeq != oldSeq) {
            delete oldSeq;
            sequence = newSeq;
        }
    }
};

class DoubleSequenceWrapper : public SequenceWrapper {
    Sequence<double>* sequence;
public:
    DoubleSequenceWrapper(Sequence<double>* seq) : sequence(seq) {}
    ~DoubleSequenceWrapper() { delete sequence; }

    void Print() const override {
        print(sequence);
    }

    const char* TypeName() const override {
        return sequence->TypeName();
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return nullptr;
    }

    Sequence<double>* GetDoubleSequence() override {
        return sequence->Clone();
    }

    Sequence<Pair<int, int>>* GetPairIntSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, double>>* GetPairDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() override {
        return nullptr;
    }
    
    void AppendInt(int value) override {
        throw MyException(5, "Cannot append int to double sequence");
    }
    
    void AppendDouble(double value) override {
        Sequence<double>* oldSeq = sequence;
        Sequence<double>* newSeq = sequence->Append(value);
        
        if (newSeq != oldSeq) {
            delete oldSeq;
            sequence = newSeq;
        }
    }
    
    void RemoveItemAt(int index) override {
        Sequence<double>* oldSeq = sequence;
        Sequence<double>* newSeq = sequence->RemoveAt(index);
        
        if (newSeq != oldSeq) {
            delete oldSeq;
            sequence = newSeq;
        }
    }
};

class PairIntSequenceWrapper : public SequenceWrapper {
    Sequence<Pair<int, int>>* sequence;
public:
    PairIntSequenceWrapper(Sequence<Pair<int, int>>* seq) : sequence(seq) {}
    ~PairIntSequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto pair = sequence->Get(i);
            cout << "(" << pair.first << ", " << pair.second << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "PairSequence<int, int>";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return nullptr;
    }

    Sequence<double>* GetDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, int>>* GetPairIntSequence() override {
        return sequence->Clone();
    }

    Sequence<Pair<double, double>>* GetPairDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() override {
        return nullptr;
    }
    
    void AppendInt(int value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void AppendDouble(double value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void RemoveItemAt(int index) override {
        throw MyException(5, "Cannot remove from pair sequence");
    }
};

class PairDoubleSequenceWrapper : public SequenceWrapper {
    Sequence<Pair<double, double>>* sequence;
public:
    PairDoubleSequenceWrapper(Sequence<Pair<double, double>>* seq) : sequence(seq) {}
    ~PairDoubleSequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto pair = sequence->Get(i);
            cout << "(" << pair.first << ", " << pair.second << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "PairSequence<double, double>";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return nullptr;
    }

    Sequence<double>* GetDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, int>>* GetPairIntSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, double>>* GetPairDoubleSequence() override {
        return sequence->Clone();
    }

    Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() override {
        return nullptr;
    }
    
    void AppendInt(int value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void AppendDouble(double value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void RemoveItemAt(int index) override {
        throw MyException(5, "Cannot remove from pair sequence");
    }
};

class PairMixedIntDoubleSequenceWrapper : public SequenceWrapper {
    Sequence<Pair<int, double>>* sequence;
public:
    PairMixedIntDoubleSequenceWrapper(Sequence<Pair<int, double>>* seq) : sequence(seq) {}
    ~PairMixedIntDoubleSequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto pair = sequence->Get(i);
            cout << "(" << pair.first << ", " << pair.second << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "PairSequence<int, double>";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return nullptr;
    }

    Sequence<double>* GetDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, int>>* GetPairIntSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, double>>* GetPairDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() override {
        return sequence->Clone();
    }

    Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() override {
        return nullptr;
    }
    
    void AppendInt(int value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void AppendDouble(double value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void RemoveItemAt(int index) override {
        throw MyException(5, "Cannot remove from pair sequence");
    }
};

class PairMixedDoubleIntSequenceWrapper : public SequenceWrapper {
    Sequence<Pair<double, int>>* sequence;
public:
    PairMixedDoubleIntSequenceWrapper(Sequence<Pair<double, int>>* seq) : sequence(seq) {}
    ~PairMixedDoubleIntSequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto pair = sequence->Get(i);
            cout << "(" << pair.first << ", " << pair.second << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "PairSequence<double, int>";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return nullptr;
    }

    Sequence<double>* GetDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, int>>* GetPairIntSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, double>>* GetPairDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<int, double>>* GetPairMixedIntDoubleSequence() override {
        return nullptr;
    }

    Sequence<Pair<double, int>>* GetPairMixedDoubleIntSequence() override {
        return sequence->Clone();
    }
    
    void AppendInt(int value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void AppendDouble(double value) override {
        throw MyException(5, "Cannot append to pair sequence");
    }
    
    void RemoveItemAt(int index) override {
        throw MyException(5, "Cannot remove from pair sequence");
    }
};

int multiplyByTwo(const int& x) {
    return x * 2;
}

int square(const int& x) {
    return x * x;
}

int Negate(const int& x) {
    return -x;
}

int addTen(const int& x) {
    return x + 10;
}

double multiplyByTwoDouble(const double& x) {
    return x * 2.0;
}

double squareDouble(const double& x) {
    return x * x;
}

double negateDouble(const double& x) {
    return -x;
}

double addTenDouble(const double& x) {
    return x + 10.0;
}

bool isEven(const int& x) {
    return x % 2 == 0;
}

bool isPositive(const int& x) {
    return x > 0;
}

bool greaterThanFive(const int& x) {
    return x > 5;
}

bool isPrime(const int& x) {
    if (x <= 1) return false;
    if (x <= 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (int i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

bool isPositiveDouble(const double& x) {
    return x > 0.0;
}

bool greaterThanFiveDouble(const double& x) {
    return x > 5.0;
}

int readInt(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    
    stringstream ss(input);
    int value;
    if (ss >> value && ss.eof()) {
        return value;
    }
    throw MyException(4, "Invalid integer input");
}

double readDouble(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin, input);
    
    stringstream ss(input);
    double value;
    if (ss >> value && ss.eof()) {
        return value;
    }
    throw MyException(4, "Invalid double input");
}

int chooseSequence(const vector<shared_ptr<SequenceWrapper>>& seqs, const string& prompt) {
    if (seqs.empty()) {
        throw MyException(3, "No sequences available");
    }
    cout << "Available sequences:\n";
    for (size_t i = 0; i < seqs.size(); i++) {
        cout << "  ID=" << i << " (" << seqs[i]->TypeName() 
             << ", len=" << seqs[i]->GetLength() << ")\n";
    }
    return readInt(prompt);
}

void handleCreate(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int typeChoice = readInt("Choose data type:\n 1) int\n 2) double\nEnter: ");
    if (typeChoice != 1 && typeChoice != 2) {
        throw MyException(4, "Invalid data type choice");
    }

    int choice = readInt("Choose type:\n 1) ArraySequence\n 2) ListSequence\n 3) ImmutableArraySequence\n 4) ImmutableListSequence\nEnter: ");

    if (typeChoice == 1) {
        switch (choice) {
        case 1: {
            auto seq = new ArraySequence<int>();
            seqs.push_back(make_shared<IntSequenceWrapper>(seq));
            cout << "[OK] Created ArraySequence<int>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        case 2: {
            auto seq = new ListSequence<int>();
            seqs.push_back(make_shared<IntSequenceWrapper>(seq));
            cout << "[OK] Created ListSequence<int>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        case 3: {
            int n = readInt("Enter initial size: ");
            if (n < 0) {
                throw MyException(1, "Negative size specified");
            }
            ArraySequence<int> tempSeq;
            for (int i = 0; i < n; i++) {
                int elem = readInt("arr[" + to_string(i) + "] = ");
                tempSeq.Append(elem);
            }
            auto seq = new ImmutableArraySequence<int>(tempSeq);
            seqs.push_back(make_shared<IntSequenceWrapper>(seq));
            cout << "[OK] Created ImmutableArraySequence<int>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        case 4: {
            int n = readInt("Enter initial size: ");
            if (n < 0) {
                throw MyException(1, "Negative size specified");
            }
            ArraySequence<int> tempSeq;
            for (int i = 0; i < n; i++) {
                int elem = readInt("arr[" + to_string(i) + "] = ");
                tempSeq.Append(elem);
            }
            auto seq = new ImmutableListSequence<int>(tempSeq);
            seqs.push_back(make_shared<IntSequenceWrapper>(seq));
            cout << "[OK] Created ImmutableListSequence<int>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        default:
            throw MyException(4, "Invalid choice");
        }
    } else if (typeChoice == 2) {
        switch (choice) {
        case 1: {
            auto seq = new ArraySequence<double>();
            seqs.push_back(make_shared<DoubleSequenceWrapper>(seq));
            cout << "[OK] Created ArraySequence<double>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        case 2: {
            auto seq = new ListSequence<double>();
            seqs.push_back(make_shared<DoubleSequenceWrapper>(seq));
            cout << "[OK] Created ListSequence<double>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        case 3: {
            int n = readInt("Enter initial size: ");
            if (n < 0) {
                throw MyException(1, "Negative size specified");
            }
            ArraySequence<double> tempSeq;
            for (int i = 0; i < n; i++) {
                double elem = readDouble("arr[" + to_string(i) + "] = ");
                tempSeq.Append(elem);
            }
            auto seq = new ImmutableArraySequence<double>(tempSeq);
            seqs.push_back(make_shared<DoubleSequenceWrapper>(seq));
            cout << "[OK] Created ImmutableArraySequence<double>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        case 4: {
            int n = readInt("Enter initial size: ");
            if (n < 0) {
                throw MyException(1, "Negative size specified");
            }
            ArraySequence<double> tempSeq;
            for (int i = 0; i < n; i++) {
                double elem = readDouble("arr[" + to_string(i) + "] = ");
                tempSeq.Append(elem);
            }
            auto seq = new ImmutableListSequence<double>(tempSeq);
            seqs.push_back(make_shared<DoubleSequenceWrapper>(seq));
            cout << "[OK] Created ImmutableListSequence<double>, ID=" << (seqs.size() - 1) << "\n";
            break;
        }
        default:
            throw MyException(4, "Invalid choice");
        }
    }
}

void handleAppend(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper = seqs[id].get();
    
    auto intWrapper = dynamic_cast<IntSequenceWrapper*>(wrapper);
    auto doubleWrapper = dynamic_cast<DoubleSequenceWrapper*>(wrapper);
    
    if (intWrapper) {
        int value = readInt("Value to append: ");
        wrapper->AppendInt(value);
        cout << "[OK] Appended " << value << " to int seq #" << id << "\n";
    } else if (doubleWrapper) {
        double value = readDouble("Value to append: ");
        wrapper->AppendDouble(value);
        cout << "[OK] Appended " << value << " to double seq #" << id << "\n";
    } else {
        throw MyException(5, "Sequence must contain int or double");
    }
}

void handlePrintAll(const vector<shared_ptr<SequenceWrapper>>& seqs) {
    if (seqs.empty()) {
        cout << "[Info] No sequences available\n";
        return;
    }
    
    cout << "All sequences:\n";
    for (size_t i = 0; i < seqs.size(); i++) {
        cout << "ID=" << i << " (" << seqs[i]->TypeName() 
             << ", len=" << seqs[i]->GetLength() << "): ";
        seqs[i]->Print();
    }
}

void handleRemoveSequence(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID to remove: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }
    
    seqs.erase(seqs.begin() + id);
    cout << "[OK] Removed sequence #" << id << "\n";
}

void handleSubsequence(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intWrapper = dynamic_cast<IntSequenceWrapper*>(seqs[id].get());
    auto doubleWrapper = dynamic_cast<DoubleSequenceWrapper*>(seqs[id].get());

    if (intWrapper) {
        int startIndex = readInt("Enter start index: ");
        int endIndex = readInt("Enter end index: ");
        
        Sequence<int>* seq = intWrapper->GetIntSequence();
        Sequence<int>* sub = seq->GetSubsequence(startIndex, endIndex);
        seqs.push_back(make_shared<IntSequenceWrapper>(sub));
        cout << "[OK] Subsequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq;
    } else if (doubleWrapper) {
        int startIndex = readInt("Enter start index: ");
        int endIndex = readInt("Enter end index: ");
        
        Sequence<double>* seq = doubleWrapper->GetDoubleSequence();
        Sequence<double>* sub = seq->GetSubsequence(startIndex, endIndex);
        seqs.push_back(make_shared<DoubleSequenceWrapper>(sub));
        cout << "[OK] Subsequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq;
    } else {
        throw MyException(5, "Sequence must contain int or double");
    }
}

void handleConcat(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id1 = chooseSequence(seqs, "Enter first sequence ID: ");
    int id2 = chooseSequence(seqs, "Enter second sequence ID: ");
    
    if (id1 < 0 || id1 >= static_cast<int>(seqs.size()) || 
        id2 < 0 || id2 >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intWrapper1 = dynamic_cast<IntSequenceWrapper*>(seqs[id1].get());
    auto intWrapper2 = dynamic_cast<IntSequenceWrapper*>(seqs[id2].get());
    auto doubleWrapper1 = dynamic_cast<DoubleSequenceWrapper*>(seqs[id1].get());
    auto doubleWrapper2 = dynamic_cast<DoubleSequenceWrapper*>(seqs[id2].get());

    if (intWrapper1 && intWrapper2) {
        Sequence<int>* seq1 = intWrapper1->GetIntSequence();
        Sequence<int>* seq2 = intWrapper2->GetIntSequence();
        Sequence<int>* result = seq1->Concat(seq2);
        
        seqs.push_back(make_shared<IntSequenceWrapper>(result));
        cout << "[OK] Concatenation added as ID=" << (seqs.size() - 1) << "\n";
        delete seq1;
        delete seq2;
    } else if (doubleWrapper1 && doubleWrapper2) {
        Sequence<double>* seq1 = doubleWrapper1->GetDoubleSequence();
        Sequence<double>* seq2 = doubleWrapper2->GetDoubleSequence();
        Sequence<double>* result = seq1->Concat(seq2);
        
        seqs.push_back(make_shared<DoubleSequenceWrapper>(result));
        cout << "[OK] Concatenation added as ID=" << (seqs.size() - 1) << "\n";
        delete seq1;
        delete seq2;
    } else {
        throw MyException(5, "Both sequences must have same type (int or double)");
    }
}

void handleZip(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id1 = chooseSequence(seqs, "Enter first sequence ID: ");
    int id2 = chooseSequence(seqs, "Enter second sequence ID: ");
    
    if (id1 < 0 || id1 >= static_cast<int>(seqs.size()) || 
        id2 < 0 || id2 >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intWrapper1 = dynamic_cast<IntSequenceWrapper*>(seqs[id1].get());
    auto intWrapper2 = dynamic_cast<IntSequenceWrapper*>(seqs[id2].get());
    auto doubleWrapper1 = dynamic_cast<DoubleSequenceWrapper*>(seqs[id1].get());
    auto doubleWrapper2 = dynamic_cast<DoubleSequenceWrapper*>(seqs[id2].get());

    if (intWrapper1 && intWrapper2) {
        Sequence<int>* seq1 = intWrapper1->GetIntSequence();
        Sequence<int>* seq2 = intWrapper2->GetIntSequence();
        auto* zipped = zip<int, int>(seq1, seq2);
        
        seqs.push_back(make_shared<PairIntSequenceWrapper>(zipped));
        cout << "[OK] Zipped pair sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq1;
        delete seq2;
    } else if (intWrapper1 && doubleWrapper2) {
        Sequence<int>* seq1 = intWrapper1->GetIntSequence();
        Sequence<double>* seq2 = doubleWrapper2->GetDoubleSequence();
        auto* zipped = zip<int, double>(seq1, seq2);
        
        seqs.push_back(make_shared<PairMixedIntDoubleSequenceWrapper>(zipped));
        cout << "[OK] Zipped mixed pair sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq1;
        delete seq2;
    } else if (doubleWrapper1 && intWrapper2) {
        Sequence<double>* seq1 = doubleWrapper1->GetDoubleSequence();
        Sequence<int>* seq2 = intWrapper2->GetIntSequence();
        auto* zipped = zip<double, int>(seq1, seq2);
        
        seqs.push_back(make_shared<PairMixedDoubleIntSequenceWrapper>(zipped));
        cout << "[OK] Zipped mixed pair sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq1;
        delete seq2;
    } else if (doubleWrapper1 && doubleWrapper2) {
        Sequence<double>* seq1 = doubleWrapper1->GetDoubleSequence();
        Sequence<double>* seq2 = doubleWrapper2->GetDoubleSequence();
        auto* zipped = zip<double, double>(seq1, seq2);
        
        seqs.push_back(make_shared<PairDoubleSequenceWrapper>(zipped));
        cout << "[OK] Zipped pair sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq1;
        delete seq2;
    } else {
        throw MyException(5, "Sequences must be of type int or double");
    }
}

void handleUnzip(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter pair sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intPairWrapper = dynamic_cast<PairIntSequenceWrapper*>(seqs[id].get());
    if (intPairWrapper) {
        Sequence<Pair<int, int>>* pairSeq = intPairWrapper->GetPairIntSequence();
        auto res = unzip<int, int>(pairSeq);
        
        delete pairSeq;
        
        seqs.push_back(make_shared<IntSequenceWrapper>(res.first));
        seqs.push_back(make_shared<IntSequenceWrapper>(res.second));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
             << " and " << (seqs.size() - 1) << "\n";
        return;
    }
    
    auto doublePairWrapper = dynamic_cast<PairDoubleSequenceWrapper*>(seqs[id].get());
    if (doublePairWrapper) {
        Sequence<Pair<double, double>>* pairSeq = doublePairWrapper->GetPairDoubleSequence();
        auto res = unzip<double, double>(pairSeq);
        
        delete pairSeq;
        
        seqs.push_back(make_shared<DoubleSequenceWrapper>(res.first));
        seqs.push_back(make_shared<DoubleSequenceWrapper>(res.second));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
             << " and " << (seqs.size() - 1) << "\n";
        return;
    }
    
    auto mixedPairWrapper1 = dynamic_cast<PairMixedIntDoubleSequenceWrapper*>(seqs[id].get());
    if (mixedPairWrapper1) {
        Sequence<Pair<int, double>>* pairSeq = mixedPairWrapper1->GetPairMixedIntDoubleSequence();
        auto res = unzip<int, double>(pairSeq);
        
        delete pairSeq;
        
        seqs.push_back(make_shared<IntSequenceWrapper>(res.first));
        seqs.push_back(make_shared<DoubleSequenceWrapper>(res.second));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
             << " and " << (seqs.size() - 1) << "\n";
        return;
    }
    
    auto mixedPairWrapper2 = dynamic_cast<PairMixedDoubleIntSequenceWrapper*>(seqs[id].get());
    if (mixedPairWrapper2) {
        Sequence<Pair<double, int>>* pairSeq = mixedPairWrapper2->GetPairMixedDoubleIntSequence();
        auto res = unzip<double, int>(pairSeq);
        
        delete pairSeq;
        
        seqs.push_back(make_shared<DoubleSequenceWrapper>(res.first));
        seqs.push_back(make_shared<IntSequenceWrapper>(res.second));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
             << " and " << (seqs.size() - 1) << "\n";
        return;
    }
    
    throw MyException(5, "Not a pair sequence");
}

void handleRemoveItem(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intWrapper = dynamic_cast<IntSequenceWrapper*>(seqs[id].get());
    auto doubleWrapper = dynamic_cast<DoubleSequenceWrapper*>(seqs[id].get());

    if (intWrapper) {
        int index = readInt("Enter index to remove: ");
        intWrapper->RemoveItemAt(index);
        cout << "[OK] Removed item at index " << index 
             << " from int seq #" << id << "\n";
    } else if (doubleWrapper) {
        int index = readInt("Enter index to remove: ");
        doubleWrapper->RemoveItemAt(index);
        cout << "[OK] Removed item at index " << index 
             << " from double seq #" << id << "\n";
    } else {
        throw MyException(5, "Sequence must contain int or double");
    }
}

void handleMap(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intWrapper = dynamic_cast<IntSequenceWrapper*>(seqs[id].get());
    auto doubleWrapper = dynamic_cast<DoubleSequenceWrapper*>(seqs[id].get());

    if (intWrapper) {
        cout << "Choose map function:\n"
             << "1) Multiply by 2\n"
             << "2) Square\n"
             << "3) Negate\n"
             << "4) Add 10\n"
             << "Enter choice: ";
        
        int choice = readInt("");
        Sequence<int>* seq = intWrapper->GetIntSequence();
        Sequence<int>* result = nullptr;
        
        switch (choice) {
        case 1:
            result = map<int, int>(seq, multiplyByTwo);
            break;
        case 2:
            result = map<int, int>(seq, square);
            break;
        case 3:
            result = map<int, int>(seq, Negate);
            break;
        case 4:
            result = map<int, int>(seq, addTen);
            break;
        default:
            throw MyException(4, "Invalid choice");
        }
        
        seqs.push_back(make_shared<IntSequenceWrapper>(result));
        cout << "[OK] Mapped sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq;
    } else if (doubleWrapper) {
        cout << "Choose map function:\n"
             << "1) Multiply by 2\n"
             << "2) Square\n"
             << "3) Negate\n"
             << "4) Add 10\n"
             << "Enter choice: ";
        
        int choice = readInt("");
        Sequence<double>* seq = doubleWrapper->GetDoubleSequence();
        Sequence<double>* result = nullptr;
        
        switch (choice) {
        case 1:
            result = map<double, double>(seq, multiplyByTwoDouble);
            break;
        case 2:
            result = map<double, double>(seq, squareDouble);
            break;
        case 3:
            result = map<double, double>(seq, negateDouble);
            break;
        case 4:
            result = map<double, double>(seq, addTenDouble);
            break;
        default:
            throw MyException(4, "Invalid choice");
        }
        
        seqs.push_back(make_shared<DoubleSequenceWrapper>(result));
        cout << "[OK] Mapped sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq;
    } else {
        throw MyException(5, "Sequence must contain int or double");
    }
}

void handleWhere(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intWrapper = dynamic_cast<IntSequenceWrapper*>(seqs[id].get());
    auto doubleWrapper = dynamic_cast<DoubleSequenceWrapper*>(seqs[id].get());

    if (intWrapper) {
        cout << "Choose filter condition:\n"
             << "1) Even numbers only\n"
             << "2) Positive numbers only\n"
             << "3) Numbers greater than 5\n"
             << "4) Prime numbers only\n"
             << "Enter choice: ";
        
        int choice = readInt("");
        Sequence<int>* seq = intWrapper->GetIntSequence();
        Sequence<int>* result = nullptr;
        
        switch (choice) {
        case 1:
            result = where<int>(seq, isEven);
            break;
        case 2:
            result = where<int>(seq, isPositive);
            break;
        case 3:
            result = where<int>(seq, greaterThanFive);
            break;
        case 4:
            result = where<int>(seq, isPrime);
            break;
        default:
            throw MyException(4, "Invalid choice");
        }
        
        seqs.push_back(make_shared<IntSequenceWrapper>(result));
        cout << "[OK] Filtered sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq;
    } else if (doubleWrapper) {
        cout << "Choose filter condition:\n"
             << "1) Positive numbers only\n"
             << "2) Numbers greater than 5\n"
             << "Enter choice: ";
        
        int choice = readInt("");
        Sequence<double>* seq = doubleWrapper->GetDoubleSequence();
        Sequence<double>* result = nullptr;
        
        switch (choice) {
        case 1:
            result = where<double>(seq, isPositiveDouble);
            break;
        case 2:
            result = where<double>(seq, greaterThanFiveDouble);
            break;
        default:
            throw MyException(4, "Invalid choice");
        }
        
        seqs.push_back(make_shared<DoubleSequenceWrapper>(result));
        cout << "[OK] Filtered sequence added as ID=" << (seqs.size() - 1) << "\n";
        delete seq;
    } else {
        throw MyException(5, "Sequence must contain int or double");
    }
}

void handleReduce(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto intWrapper = dynamic_cast<IntSequenceWrapper*>(seqs[id].get());
    auto doubleWrapper = dynamic_cast<DoubleSequenceWrapper*>(seqs[id].get());

    if (intWrapper) {
        cout << "Choose reduce operation:\n"
             << "1) Sum\n"
             << "2) Product\n"
             << "3) Maximum\n"
             << "4) Minimum\n"
             << "Enter choice: ";
        
        int choice = readInt("");
        Sequence<int>* seq = intWrapper->GetIntSequence();
        int result = 0;
        
        switch (choice) {
        case 1: {
            int sum = reduce<int>(seq, [](const int& a, const int& b) { return a + b; }, 0);
            result = sum;
            break;
        }
        case 2: {
            int product = reduce<int>(seq, [](const int& a, const int& b) { return a * b; }, 1);
            result = product;
            break;
        }
        case 3: {
            if (seq->GetLength() == 0) {
                throw MyException(3, "Sequence is empty");
            }
            int maxVal = reduce<int>(seq, [](const int& a, const int& b) { return a > b ? a : b; }, seq->Get(0));
            result = maxVal;
            break;
        }
        case 4: {
            if (seq->GetLength() == 0) {
                throw MyException(3, "Sequence is empty");
            }
            int minVal = reduce<int>(seq, [](const int& a, const int& b) { return a < b ? a : b; }, seq->Get(0));
            result = minVal;
            break;
        }
        default:
            throw MyException(4, "Invalid choice");
        }
        
        cout << "[OK] Reduce result: " << result << "\n";
        delete seq;
    } else if (doubleWrapper) {
        cout << "Choose reduce operation:\n"
             << "1) Sum\n"
             << "2) Product\n"
             << "3) Maximum\n"
             << "4) Minimum\n"
             << "Enter choice: ";
        
        int choice = readInt("");
        Sequence<double>* seq = doubleWrapper->GetDoubleSequence();
        double result = 0.0;
        
        switch (choice) {
        case 1: {
            double sum = reduce<double>(seq, [](const double& a, const double& b) { return a + b; }, 0.0);
            result = sum;
            break;
        }
        case 2: {
            double product = reduce<double>(seq, [](const double& a, const double& b) { return a * b; }, 1.0);
            result = product;
            break;
        }
        case 3: {
            if (seq->GetLength() == 0) {
                throw MyException(3, "Sequence is empty");
            }
            double maxVal = reduce<double>(seq, [](const double& a, const double& b) { return a > b ? a : b; }, seq->Get(0));
            result = maxVal;
            break;
        }
        case 4: {
            if (seq->GetLength() == 0) {
                throw MyException(3, "Sequence is empty");
            }
            double minVal = reduce<double>(seq, [](const double& a, const double& b) { return a < b ? a : b; }, seq->Get(0));
            result = minVal;
            break;
        }
        default:
            throw MyException(4, "Invalid choice");
        }
        
        cout << "[OK] Reduce result: " << result << "\n";
        delete seq;
    } else {
        throw MyException(5, "Sequence must contain int or double");
    }
}

void runUI() {
    vector<shared_ptr<SequenceWrapper>> seqs;
    bool running = true;

    while (running) {
        try {
            cout << "\n==== MENU ====\n"
                 << "1) Create sequence\n"
                 << "2) Append\n"
                 << "3) Print ALL\n"
                 << "4) Remove sequence\n"
                 << "5) Subsequence\n"
                 << "6) Concat\n"
                 << "7) Zip (pair)\n"
                 << "8) Unzip (pair)\n"
                 << "9) Remove item\n"
                 << "10) Map (transform)\n"
                 << "11) Where (filter)\n"
                 << "12) Reduce (aggregate)\n"
                 << "0) Exit\n"
                 << "Choose: ";

            int cmd = readInt("");
            switch (cmd) {
            case 1: handleCreate(seqs); break;
            case 2: handleAppend(seqs); break;
            case 3: handlePrintAll(seqs); break;
            case 4: handleRemoveSequence(seqs); break;
            case 5: handleSubsequence(seqs); break;
            case 6: handleConcat(seqs); break;
            case 7: handleZip(seqs); break;
            case 8: handleUnzip(seqs); break;
            case 9: handleRemoveItem(seqs); break;
            case 10: handleMap(seqs); break;
            case 11: handleWhere(seqs); break;
            case 12: handleReduce(seqs); break;
            case 0: running = false; break;
            default: cout << "[Warn] Unknown command\n";
            }
        }
        catch (const MyException& ex) {
            cout << "[Error] " << ex.what() << "\n";
        }
        catch (const exception& ex) {
            cout << "[Error] " << ex.what() << "\n";
        }
    }

    seqs.clear();
    cout << "Program finished.\n";
}