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

#include "Exeption.h"
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

template <typename... Ts>
class SequenceWrapper<MonadTuple<Ts...>> : public SequenceBase {
    Sequence<MonadTuple<Ts...>>* sequence;
public:
    SequenceWrapper(Sequence<MonadTuple<Ts...>>* seq) : sequence(seq) {}
    ~SequenceWrapper() { delete sequence; }

    void Print() const override {
        print(sequence);
    }

    const char* TypeName() const override {
        return "MonadTupleSequence";
    }

    int GetLength() const override {
        return sequence->GetLength();
    }
    
    // Добавлен метод get()
    Sequence<MonadTuple<Ts...>>* get() const { 
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

// Реализации handleCreate, handleAppend, handlePrintAll, 
// handleRemoveSequence, handleSubsequence, handleConcat, 
// handleZip, handleUnzip, handleRemoveItem остаются без изменений

static void handleZipAsTuple(vector<shared_ptr<SequenceBase>>& seqs) {
    if (seqs.empty()) {
        cout << "[Warn] No sequences available\n";
        return;
    }
    
    auto countOption = readInt("Enter number of sequences to zip (min 2): ");
    if (countOption.IsNone() || countOption.Unwrap() < 2) {
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
    
    // Создаем кортежную последовательность
    shared_ptr<SequenceBase> result;
    
    // Обработка разного количества последовательностей
    switch (count) {
        case 2: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1]);
            result = make_shared<SequenceWrapper<MonadTuple<int, int>>>(zipped);
            break;
        }
        case 3: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1], sequences[2]);
            result = make_shared<SequenceWrapper<MonadTuple<int, int, int>>>(zipped);
            break;
        }
        case 4: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1], sequences[2], sequences[3]);
            result = make_shared<SequenceWrapper<MonadTuple<int, int, int, int>>>(zipped);
            break;
        }
        case 5: {
            auto* zipped = zip_as_tuple(sequences[0], sequences[1], sequences[2], sequences[3], sequences[4]);
            result = make_shared<SequenceWrapper<MonadTuple<int, int, int, int, int>>>(zipped);
            break;
        }
        default:
            cout << "[Error] Maximum 5 sequences supported\n";
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

    // Определение размера кортежа
    if (auto wrapper2 = dynamic_cast<SequenceWrapper<MonadTuple<int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper2->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<0>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<1>(res)));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 2)
             << " and " << (seqs.size() - 1) << "\n";
    }
    else if (auto wrapper3 = dynamic_cast<SequenceWrapper<MonadTuple<int, int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper3->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<0>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<1>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<2>(res)));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 3)
             << ", " << (seqs.size() - 2) << " and " << (seqs.size() - 1) << "\n";
    }
    else if (auto wrapper4 = dynamic_cast<SequenceWrapper<MonadTuple<int, int, int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper4->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<0>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<1>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<2>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<3>(res)));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 4) << ", " << (seqs.size() - 3)
             << ", " << (seqs.size() - 2) << " and " << (seqs.size() - 1) << "\n";
    }
    else if (auto wrapper5 = dynamic_cast<SequenceWrapper<MonadTuple<int, int, int, int, int>>*>(seqs[id].get())) {
        auto res = unzip_tuple(wrapper5->get());
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<0>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<1>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<2>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<3>(res)));
        seqs.push_back(make_shared<SequenceWrapper<int>>(get<4>(res)));
        cout << "[OK] Unzipped to IDs " << (seqs.size() - 5) << ", " << (seqs.size() - 4)
             << ", " << (seqs.size() - 3) << ", " << (seqs.size() - 2) 
             << " and " << (seqs.size() - 1) << "\n";
    }
    else {
        cout << "[Error] Unsupported tuple sequence type\n";
    }
}