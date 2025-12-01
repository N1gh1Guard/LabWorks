#include "UI.h"
#include <iostream>
#include <vector>
#include <memory>
#include <functional>
#include <sstream>
#include <utility>

#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"

#include "Exception.h"
#include "Error.h"
#include "Functions.h"
#include "Option.h"
#include "MonadTuple.h"

using namespace std;

class SequenceBase {
public:
    virtual void Print() const = 0;
    virtual const char* TypeName() const = 0;
    virtual int GetLength() const = 0;
    virtual ~SequenceBase() = default;
};

template <typename T>
class SequenceWrapper : public SequenceBase {
    Sequence<T>* sequence;
public:
    SequenceWrapper(Sequence<T>* seq) : sequence(seq) {}
    ~SequenceWrapper() { delete sequence; }

    void Print() const override {
        print(sequence);
    }

    const char* TypeName() const override {
        return sequence->TypeName();
    }

    int GetLength() const override {
        return sequence->GetLength();
    }

    Sequence<T>* get() const { return sequence; }
};

template <typename T1, typename T2>
class SequenceWrapper<MonadTuple2<T1, T2>> : public SequenceBase {
    Sequence<MonadTuple2<T1, T2>>* sequence;
public:
    SequenceWrapper(Sequence<MonadTuple2<T1, T2>>* seq) : sequence(seq) {}
    ~SequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto tuple = sequence->Get(i);
            cout << "(" << tuple.first << ", " << tuple.second << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "MonadTuple2Sequence";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }
    
    Sequence<MonadTuple2<T1, T2>>* get() const { 
        return sequence; 
    }
};

template <typename T1, typename T2, typename T3>
class SequenceWrapper<MonadTuple3<T1, T2, T3>> : public SequenceBase {
    Sequence<MonadTuple3<T1, T2, T3>>* sequence;
public:
    SequenceWrapper(Sequence<MonadTuple3<T1, T2, T3>>* seq) : sequence(seq) {}
    ~SequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto tuple = sequence->Get(i);
            cout << "(" << tuple.first << ", " << tuple.second << ", " << tuple.third << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "MonadTuple3Sequence";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }
    
    Sequence<MonadTuple3<T1, T2, T3>>* get() const { 
        return sequence; 
    }
};

template <typename T1, typename T2, typename T3, typename T4>
class SequenceWrapper<MonadTuple4<T1, T2, T3, T4>> : public SequenceBase {
    Sequence<MonadTuple4<T1, T2, T3, T4>>* sequence;
public:
    SequenceWrapper(Sequence<MonadTuple4<T1, T2, T3, T4>>* seq) : sequence(seq) {}
    ~SequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto tuple = sequence->Get(i);
            cout << "(" << tuple.first << ", " << tuple.second << ", " << tuple.third << ", " << tuple.fourth << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "MonadTuple4Sequence";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }
    
    Sequence<MonadTuple4<T1, T2, T3, T4>>* get() const { 
        return sequence; 
    }
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class SequenceWrapper<MonadTuple5<T1, T2, T3, T4, T5>> : public SequenceBase {
    Sequence<MonadTuple5<T1, T2, T3, T4, T5>>* sequence;
public:
    SequenceWrapper(Sequence<MonadTuple5<T1, T2, T3, T4, T5>>* seq) : sequence(seq) {}
    ~SequenceWrapper() { delete sequence; }

    void Print() const override {
        cout << "[ ";
        for (int i = 0; i < sequence->GetLength(); i++) {
            if (i > 0) cout << ", ";
            auto tuple = sequence->Get(i);
            cout << "(" << tuple.first << ", " << tuple.second << ", " << tuple.third << ", " << tuple.fourth << ", " << tuple.fifth << ")";
        }
        cout << " ]\n";
    }

    const char* TypeName() const override {
        return "MonadTuple5Sequence";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }
    
    Sequence<MonadTuple5<T1, T2, T3, T4, T5>>* get() const { 
        return sequence; 
    }
};

template <typename T1, typename T2>
class SequenceWrapper<MonadPair<T1, T2>> : public SequenceBase {
    Sequence<MonadPair<T1, T2>>* sequence;
public:
    SequenceWrapper(Sequence<MonadPair<T1, T2>>* seq) : sequence(seq) {}
    ~SequenceWrapper() { delete sequence; }

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
        return "MonadPairSequence";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }
    
    Sequence<MonadPair<T1, T2>>* get() const { 
        return sequence; 
    }
};


Option<int> readInt(const string& prompt) {
    cout << prompt;
    int value;
    if (cin >> value) return Option<int>::Some(value);
    cin.clear();
    cin.ignore(10000, '\n');
    return Option<int>::None();
}

static void handleCreate(vector<shared_ptr<SequenceBase>>& seqs);
static void handleAppend(vector<shared_ptr<SequenceBase>>& seqs);
static void handlePrintAll(const vector<shared_ptr<SequenceBase>>& seqs);
static void handleRemoveSequence(vector<shared_ptr<SequenceBase>>& seqs);
static void handleSubsequence(vector<shared_ptr<SequenceBase>>& seqs);
static void handleConcat(vector<shared_ptr<SequenceBase>>& seqs);
static void handleZip(vector<shared_ptr<SequenceBase>>& seqs);
static void handleUnzip(vector<shared_ptr<SequenceBase>>& seqs);
static void handleRemoveItem(vector<shared_ptr<SequenceBase>>& seqs);
static void handleZipAsTuple(vector<shared_ptr<SequenceBase>>& seqs);
static void handleUnzipTuple(vector<shared_ptr<SequenceBase>>& seqs);

Option<int> chooseSequence(const vector<shared_ptr<SequenceBase>>& seqs, const string& prompt) {
    if (seqs.empty()) {
        cout << "[Info] No sequences available\n";
        return Option<int>::None();
    }
    
    cout << "Available sequences:\n";
    for (size_t i = 0; i < seqs.size(); i++) {
        cout << "  ID=" << i << " (" << seqs[i]->TypeName() 
             << ", len=" << seqs[i]->GetLength() << ")\n";
    }
    
    return readInt(prompt);
}

void runUI() {
    vector<shared_ptr<SequenceBase>> seqs;
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
                 << "10) Zip as Tuple (multiple)\n"
                 << "11) Unzip Tuple (multiple)\n"
                 << "0) Exit\n"
                 << "Choose: ";

            auto cmdOption = readInt("");
            if (cmdOption.IsNone()) {
                cout << "[Error] Invalid input\n";
                continue;
            }

            int cmd = cmdOption.Unwrap();
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
            case 10: handleZipAsTuple(seqs); break;
            case 11: handleUnzipTuple(seqs); break;
            case 0: running = false; break;
            default: cout << "[Warn] Unknown command\n";
            }
        }
        catch (const exception& ex) {
            cout << "[Error] " << ex.what() << "\n";
        }
    }

    seqs.clear();
    cout << "Program finished.\n";
}

static void handleCreate(vector<shared_ptr<SequenceBase>>& seqs) {
    auto choice = readInt("Choose type:\n"
                         " 1) ArraySequence\n"
                         " 2) ListSequence\n"
                         " 3) ImmutableArraySequence\n"
                         " 4) ImmutableListSequence\n"
                         "Enter: ");
    if (choice.IsNone()) {
        cout << "[Error] Invalid choice\n";
        return;
    }

    switch (choice.Unwrap()) {
    case 1: {
        auto seq = new ArraySequence<int>();
        seqs.push_back(make_shared<SequenceWrapper<int>>(seq));
        cout << "[OK] Created ArraySequence, ID=" << (seqs.size() - 1) << "\n";
        break;
    }
    case 2: {
        auto seq = new ListSequence<int>();
        seqs.push_back(make_shared<SequenceWrapper<int>>(seq));
        cout << "[OK] Created ListSequence, ID=" << (seqs.size() - 1) << "\n";
        break;
    }
    case 3: {
        auto nOption = readInt("Enter initial size: ");
        if (nOption.IsNone() || nOption.Unwrap() < 0) {
            cout << "[Error] Invalid size\n";
            return;
        }
        int n = nOption.Unwrap();
        ArraySequence<int> tempSeq;
        for (int i = 0; i < n; i++) {
            auto elemOption = readInt("arr[" + to_string(i) + "] = ");
            if (elemOption.IsNone()) {
                cout << "[Error] Invalid element\n";
                return;
            }
            tempSeq.Append(elemOption.Unwrap());
        }
        auto seq = new ImmutableArraySequence<int>(tempSeq);
        seqs.push_back(make_shared<SequenceWrapper<int>>(seq));
        cout << "[OK] Created ImmutableArraySequence, ID=" << (seqs.size() - 1) << "\n";
        break;
    }
    case 4: {
        auto nOption = readInt("Enter initial size: ");
        if (nOption.IsNone() || nOption.Unwrap() < 0) {
            cout << "[Error] Invalid size\n";
            return;
        }
        int n = nOption.Unwrap();
        ArraySequence<int> tempSeq;
        for (int i = 0; i < n; i++) {
            auto elemOption = readInt("arr[" + to_string(i) + "] = ");
            if (elemOption.IsNone()) {
                cout << "[Error] Invalid element\n";
                return;
            }
            tempSeq.Append(elemOption.Unwrap());
        }
        auto seq = new ImmutableListSequence<int>(tempSeq);
        seqs.push_back(make_shared<SequenceWrapper<int>>(seq));
        cout << "[OK] Created ImmutableListSequence, ID=" << (seqs.size() - 1) << "\n";
        break;
    }
    default:
        cout << "[Error] Invalid choice\n";
    }
}

static void handleAppend(vector<shared_ptr<SequenceBase>>& seqs) {
    auto idOption = chooseSequence(seqs, "Enter sequence ID: ");
    if (idOption.IsNone()) return;
    
    int id = idOption.Unwrap();
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    auto wrapper = dynamic_cast<SequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        cout << "[Error] Sequence must contain integers\n";
        return;
    }

    auto valOption = readInt("Value to append: ");
    if (valOption.IsNone()) {
        cout << "[Error] Invalid value\n";
        return;
    }

    Sequence<int>* oldSeq = wrapper->get();
    Sequence<int>* newSeq = oldSeq->Append(valOption.Unwrap());
    
    if (newSeq != oldSeq) {
        auto newWrapper = new SequenceWrapper<int>(newSeq);
        seqs[id].reset(newWrapper);
        cout << "[OK] Appended " << valOption.Unwrap() << " to seq #" << id << " (new immutable sequence)\n";
    } else {
        cout << "[OK] Appended " << valOption.Unwrap() << " to seq #" << id << " (mutable sequence)\n";
    }
}

static void handleRemoveSequance(vector<shared_ptr<SequenceBase>>& seqs) {
    auto idOption = chooseSequence(seqs, "Enter sequence ID: ");
    if (idOption.IsNone()) return;
    
    int id = idOption.Unwrap();
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    auto wrapper = dynamic_cast<SequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        cout << "[Error] Sequence must contain integers\n";
        return;
    }

    auto idxOption = readInt("Enter index to remove: ");
    if (idxOption.IsNone()) {
        cout << "[Error] Invalid index\n";
        return;
    }

    Sequence<int>* oldSeq = wrapper->get();
    Sequence<int>* newSeq = oldSeq->RemoveAt(idxOption.Unwrap());
    
    if (newSeq != oldSeq) {
        auto newWrapper = new SequenceWrapper<int>(newSeq);
        seqs[id].reset(newWrapper);
        cout << "[OK] Removed item at index " << idxOption.Unwrap() 
             << " from seq #" << id << " (new immutable sequence)\n";
    } else {
        cout << "[OK] Removed item at index " << idxOption.Unwrap() 
             << " from seq #" << id << " (mutable sequence)\n";
    }
}

static void handleSubsequence(vector<shared_ptr<SequenceBase>>& seqs) {
    auto idOption = chooseSequence(seqs, "Enter sequence ID: ");
    if (idOption.IsNone()) return;
    
    int id = idOption.Unwrap();
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    auto wrapper = dynamic_cast<SequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        cout << "[Error] Sequence must contain integers\n";
        return;
    }

    auto stOption = readInt("Enter start index: ");
    auto enOption = readInt("Enter end index: ");
    if (stOption.IsNone() || enOption.IsNone()) {
        cout << "[Error] Invalid indices\n";
        return;
    }

    int st = stOption.Unwrap();
    int en = enOption.Unwrap();
    Sequence<int>* sub = wrapper->get()->GetSubsequence(st, en);
    seqs.push_back(make_shared<SequenceWrapper<int>>(sub));
    cout << "[OK] Subsequence added as ID=" << (seqs.size() - 1) << "\n";
}

static void handleConcat(vector<shared_ptr<SequenceBase>>& seqs) {
    auto id1Option = chooseSequence(seqs, "Enter first sequence ID: ");
    auto id2Option = chooseSequence(seqs, "Enter second sequence ID: ");
    if (id1Option.IsNone() || id2Option.IsNone()) return;
    
    int id1 = id1Option.Unwrap();
    int id2 = id2Option.Unwrap();
    if (id1 < 0 || id1 >= static_cast<int>(seqs.size()) || 
        id2 < 0 || id2 >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    auto wrapper1 = dynamic_cast<SequenceWrapper<int>*>(seqs[id1].get());
    auto wrapper2 = dynamic_cast<SequenceWrapper<int>*>(seqs[id2].get());
    if (!wrapper1 || !wrapper2) {
        cout << "[Error] Both sequences must contain integers\n";
        return;
    }

    Sequence<int>* result = wrapper1->get()->Concat(wrapper2->get());
    seqs.push_back(make_shared<SequenceWrapper<int>>(result));
    cout << "[OK] Concatenation added as ID=" << (seqs.size() - 1) << "\n";
}

static void handleZip(vector<shared_ptr<SequenceBase>>& seqs) {
    auto id1Option = chooseSequence(seqs, "Enter first sequence ID: ");
    auto id2Option = chooseSequence(seqs, "Enter second sequence ID: ");
    if (id1Option.IsNone() || id2Option.IsNone()) return;
    
    int id1 = id1Option.Unwrap();
    int id2 = id2Option.Unwrap();
    if (id1 < 0 || id1 >= static_cast<int>(seqs.size()) || 
        id2 < 0 || id2 >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    auto wrapper1 = dynamic_cast<SequenceWrapper<int>*>(seqs[id1].get());
    auto wrapper2 = dynamic_cast<SequenceWrapper<int>*>(seqs[id2].get());
    if (!wrapper1 || !wrapper2) {
        cout << "[Error] Both sequences must contain integers\n";
        return;
    }

    auto* zipped = zip<int, int>(wrapper1->get(), wrapper2->get());
    seqs.push_back(make_shared<SequenceWrapper<MonadPair<int, int>>>(zipped));
    cout << "[OK] Zipped pair sequence added as ID=" << (seqs.size() - 1) << "\n";
}

static void handleUnzip(vector<shared_ptr<SequenceBase>>& seqs) {
    auto idOption = chooseSequence(seqs, "Enter pair sequence ID: ");
    if (idOption.IsNone()) return;
    
    int id = idOption.Unwrap();
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    auto wrapper = dynamic_cast<SequenceWrapper<MonadPair<int, int>>*>(seqs[id].get());
    if (!wrapper) {
        cout << "[Error] Not a pair sequence\n";
        return;
    }

    auto res = unzip<int, int>(wrapper->get());
    seqs.push_back(make_shared<SequenceWrapper<int>>(res.first));
    seqs.push_back(make_shared<SequenceWrapper<int>>(res.second));
    cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
         << " and " << (seqs.size() - 1) << "\n";
}

static void handleRemoveItem(vector<shared_ptr<SequenceBase>>& seqs) {
    auto idOption = chooseSequence(seqs, "Enter sequence ID: ");
    if (idOption.IsNone()) return;
    
    int id = idOption.Unwrap();
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    auto wrapper = dynamic_cast<SequenceWrapper<int>*>(seqs[id].get());
    if (!wrapper) {
        cout << "[Error] Sequence must contain integers\n";
        return;
    }

    auto idxOption = readInt("Enter index to remove: ");
    if (idxOption.IsNone()) {
        cout << "[Error] Invalid index\n";
        return;
    }

    Sequence<int>* newSeq = wrapper->get()->RemoveAt(idxOption.Unwrap());
    seqs[id] = make_shared<SequenceWrapper<int>>(newSeq);
    cout << "[OK] Removed item at index " << idxOption.Unwrap() 
         << " from seq #" << id << "\n";
}

static void handleZipAsTuple(vector<shared_ptr<SequenceBase>>& seqs) {
    if (seqs.empty()) {
        cout << "[Warn] No sequences available\n";
        return;
    }
    
    auto countOption = readInt("Enter number of sequences to zip (min 2, max 5): ");
    if (countOption.IsNone() || countOption.Unwrap() < 2 || countOption.Unwrap() > 5) {
        cout << "[Error] Invalid number\n";
        return;
    }
    
    int count = countOption.Unwrap();
    vector<int> ids;
    vector<Sequence<int>*> sequences;
    
    for (int i = 0; i < count; i++) {
        auto idOption = chooseSequence(seqs, "Enter sequence ID " + to_string(i+1) + ": ");
        if (idOption.IsNone()) return;
        
        int id = idOption.Unwrap();
        if (id < 0 || id >= static_cast<int>(seqs.size())) {
            cout << "[Error] Invalid sequence ID\n";
            return;
        }
        
        auto wrapper = dynamic_cast<SequenceWrapper<int>*>(seqs[id].get());
        if (!wrapper) {
            cout << "[Error] Sequence must contain integers\n";
            return;
        }
        
        ids.push_back(id);
        sequences.push_back(wrapper->get());
    }
    
    shared_ptr<SequenceBase> result;
    
    switch (count) {
        case 2: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1]);
            result = make_shared<SequenceWrapper<MonadTuple2<int, int>>>(zipped);
            break;
        }
        case 3: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1], sequences[2]);
            result = make_shared<SequenceWrapper<MonadTuple3<int, int, int>>>(zipped);
            break;
        }
        case 4: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1], sequences[2], sequences[3]);
            result = make_shared<SequenceWrapper<MonadTuple4<int, int, int, int>>>(zipped);
            break;
        }
        case 5: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1], sequences[2], sequences[3], sequences[4]);
            result = make_shared<SequenceWrapper<MonadTuple5<int, int, int, int, int>>>(zipped);
            break;
        }
        default:
            cout << "[Error] Unsupported number of sequences\n";
            return;
    }
    
    seqs.push_back(result);
    cout << "[OK] Zipped tuple sequence added as ID=" << (seqs.size() - 1) << "\n";
}

static void handleUnzipTuple(vector<shared_ptr<SequenceBase>>& seqs) {
    auto idOption = chooseSequence(seqs, "Enter tuple sequence ID: ");
    if (idOption.IsNone()) return;
    
    int id = idOption.Unwrap();
    if (id < 0 || id >= static_cast<int>(seqs.size())) {
        cout << "[Error] Invalid sequence ID\n";
        return;
    }

    if (auto wrapper2 = dynamic_cast<SequenceWrapper<MonadTuple2<int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper2->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.first));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.second));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
             << " and " << (seqs.size() - 1) << "\n";
    }
    else if (auto wrapper3 = dynamic_cast<SequenceWrapper<MonadTuple3<int, int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper3->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.first));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.second));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.third));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 3)
             << ", " << (seqs.size() - 2) << " and " << (seqs.size() - 1) << "\n";
    }
    else if (auto wrapper4 = dynamic_cast<SequenceWrapper<MonadTuple4<int, int, int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper4->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.first));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.second));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.third));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.fourth));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 4) << ", " << (seqs.size() - 3)
             << ", " << (seqs.size() - 2) << " and " << (seqs.size() - 1) << "\n";
    }
    else if (auto wrapper5 = dynamic_cast<SequenceWrapper<MonadTuple5<int, int, int, int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper5->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.first));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.second));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.third));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.fourth));
        seqs.push_back(make_shared<SequenceWrapper<int>>(res.fifth));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 5) << ", " << (seqs.size() - 4)
             << ", " << (seqs.size() - 3) << ", " << (seqs.size() - 2) 
             << " and " << (seqs.size() - 1) << "\n";
    }
    else {
        cout << "[Error] Unsupported tuple sequence type\n";
    }
}