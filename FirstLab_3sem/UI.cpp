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
};

template <>
class TypedSequenceWrapper<int> : public SequenceWrapper {
    Sequence<int>* sequence;
public:
    TypedSequenceWrapper(Sequence<int>* seq) : sequence(seq) {}
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
        return sequence;
    }
};

template <typename T1, typename T2>
class PairSequenceWrapper : public SequenceWrapper {
    Sequence<Pair<T1, T2>>* sequence;
public:
    PairSequenceWrapper(Sequence<Pair<T1, T2>>* seq) : sequence(seq) {}
    ~PairSequenceWrapper() { delete sequence; }

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
        return "PairSequence";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<int>* GetIntSequence() override {
        return nullptr;
    }

    Sequence<Pair<T1, T2>>* GetPairSequence() {
        return sequence->Clone();
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
    int choice = readInt("Choose type:\n 1) ArraySequence\n 2) ListSequence\n 3) ImmutableArraySequence\n 4) ImmutableListSequence\nEnter: ");

    switch (choice) {
    case 1: {
        auto seq = new ArraySequence<int>();
        seqs.push_back(make_shared<TypedSequenceWrapper<int>>(seq));
        cout << "[OK] Created ArraySequence, ID=" << (seqs.size() - 1) << "\n";
        break;
    }
    case 2: {
        auto seq = new ListSequence<int>();
        seqs.push_back(make_shared<TypedSequenceWrapper<int>>(seq));
        cout << "[OK] Created ListSequence, ID=" << (seqs.size() - 1) << "\n";
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
        seqs.push_back(make_shared<TypedSequenceWrapper<int>>(seq));
        cout << "[OK] Created ImmutableArraySequence, ID=" << (seqs.size() - 1) << "\n";
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
        seqs.push_back(make_shared<TypedSequenceWrapper<int>>(seq));
        cout << "[OK] Created ImmutableListSequence, ID=" << (seqs.size() - 1) << "\n";
        break;
    }
    default:
        throw MyException(4, "Invalid choice");
    }
}

void handleAppend(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        throw MyException(5, "Sequence must contain integers");
    }

    int value = readInt("Value to append: ");
    Sequence<int>* oldSeq = wrapper->GetIntSequence();
    Sequence<int>* newSeq = oldSeq->Append(value);
    
    if (newSeq != oldSeq) {
        delete oldSeq;
        seqs[id].reset(new TypedSequenceWrapper<int>(newSeq));
        cout << "[OK] Appended " << value << " to seq #" << id << " (new immutable sequence)\n";
    } else {
        cout << "[OK] Appended " << value << " to seq #" << id << " (mutable sequence)\n";
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

    auto wrapper = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        throw MyException(5, "Sequence must contain integers");
    }

    int startIndex = readInt("Enter start index: ");
    int endIndex = readInt("Enter end index: ");
    
    Sequence<int>* seq = wrapper->GetIntSequence();
    Sequence<int>* sub = seq->GetSubsequence(startIndex, endIndex);
    seqs.push_back(make_shared<TypedSequenceWrapper<int>>(sub));
    cout << "[OK] Subsequence added as ID=" << (seqs.size() - 1) << "\n";
}

void handleConcat(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id1 = chooseSequence(seqs, "Enter first sequence ID: ");
    int id2 = chooseSequence(seqs, "Enter second sequence ID: ");
    
    if (id1 < 0 || id1 >= static_cast<int>(seqs.size()) || 
        id2 < 0 || id2 >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper1 = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id1].get());
    auto wrapper2 = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id2].get());
    if (!wrapper1 || !wrapper2) {
        throw MyException(5, "Both sequences must contain integers");
    }

    Sequence<int>* seq1 = wrapper1->GetIntSequence();
    Sequence<int>* seq2 = wrapper2->GetIntSequence();
    Sequence<int>* result = seq1->Concat(seq2);
    
    seqs.push_back(make_shared<TypedSequenceWrapper<int>>(result));
    cout << "[OK] Concatenation added as ID=" << (seqs.size() - 1) << "\n";
}

void handleZip(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id1 = chooseSequence(seqs, "Enter first sequence ID: ");
    int id2 = chooseSequence(seqs, "Enter second sequence ID: ");
    
    if (id1 < 0 || id1 >= static_cast<int>(seqs.size()) || 
        id2 < 0 || id2 >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper1 = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id1].get());
    auto wrapper2 = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id2].get());
    if (!wrapper1 || !wrapper2) {
        throw MyException(5, "Both sequences must contain integers");
    }

    Sequence<int>* seq1 = wrapper1->GetIntSequence();
    Sequence<int>* seq2 = wrapper2->GetIntSequence();
    auto* zipped = zip<int, int>(seq1, seq2);
    
    seqs.push_back(make_shared<PairSequenceWrapper<int, int>>(zipped));
    cout << "[OK] Zipped pair sequence added as ID=" << (seqs.size() - 1) << "\n";
}

void handleUnzip(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter pair sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper = dynamic_cast<PairSequenceWrapper<int, int>*>(seqs[id].get());
    if (!wrapper) {
        throw MyException(5, "Not a pair sequence");
    }

    Sequence<Pair<int, int>>* pairSeq = wrapper->GetPairSequence();
    auto res = unzip<int, int>(pairSeq);
    
    delete pairSeq;
    
    seqs.push_back(make_shared<TypedSequenceWrapper<int>>(res.first));
    seqs.push_back(make_shared<TypedSequenceWrapper<int>>(res.second));
    cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
         << " and " << (seqs.size() - 1) << "\n";
}

void handleRemoveItem(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        throw MyException(5, "Sequence must contain integers");
    }

    int index = readInt("Enter index to remove: ");
    Sequence<int>* oldSeq = wrapper->GetIntSequence();
    Sequence<int>* newSeq = oldSeq->RemoveAt(index);
    
    if (newSeq != oldSeq) {
        delete oldSeq;
        seqs[id].reset(new TypedSequenceWrapper<int>(newSeq));
        cout << "[OK] Removed item at index " << index 
             << " from seq #" << id << " (new immutable sequence)\n";
    } else {
        cout << "[OK] Removed item at index " << index 
             << " from seq #" << id << " (mutable sequence)\n";
    }
}

void handleMap(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        throw MyException(5, "Sequence must contain integers");
    }

    cout << "Choose map function:\n"
         << "1) Multiply by 2\n"
         << "2) Square\n"
         << "3) Negate\n"
         << "4) Add 10\n"
         << "Enter choice: ";
    
    int choice = readInt("");
    Sequence<int>* seq = wrapper->GetIntSequence();
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
    
    seqs.push_back(make_shared<TypedSequenceWrapper<int>>(result));
    cout << "[OK] Mapped sequence added as ID=" << (seqs.size() - 1) << "\n";
}

void handleWhere(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        throw MyException(5, "Sequence must contain integers");
    }

    cout << "Choose filter condition:\n"
         << "1) Even numbers only\n"
         << "2) Positive numbers only\n"
         << "3) Numbers greater than 5\n"
         << "4) Prime numbers only\n"
         << "Enter choice: ";
    
    int choice = readInt("");
    Sequence<int>* seq = wrapper->GetIntSequence();
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
    
    seqs.push_back(make_shared<TypedSequenceWrapper<int>>(result));
    cout << "[OK] Filtered sequence added as ID=" << (seqs.size() - 1) << "\n";
}

void handleReduce(vector<shared_ptr<SequenceWrapper>>& seqs) {
    int id = chooseSequence(seqs, "Enter sequence ID: ");
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        throw MyException(2, "Invalid sequence ID");
    }

    auto wrapper = dynamic_cast<TypedSequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        throw MyException(5, "Sequence must contain integers");
    }

    cout << "Choose reduce operation:\n"
         << "1) Sum\n"
         << "2) Product\n"
         << "3) Maximum\n"
         << "4) Minimum\n"
         << "Enter choice: ";
    
    int choice = readInt("");
    Sequence<int>* seq = wrapper->GetIntSequence();
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