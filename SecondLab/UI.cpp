#include "UI.h"
#include <iostream>
#include <vector>

#include "Sequence.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"

#include "Exeption.h"
#include "Error.h"

#include "Functions.h"

static void handleCreate(std::vector<Sequence<int>*>& seqs);
static void handleAppend(std::vector<Sequence<int>*>& seqs);
static void handlePrintAll(const std::vector<Sequence<int>*>& seqs);
static void handleRemoveSequence(std::vector<Sequence<int>*>& seqs);
static void handleSubsequence(std::vector<Sequence<int>*>& seqs);
static void handleConcat(std::vector<Sequence<int>*>& seqs);
static void handleZip(std::vector<Sequence<int>*>& seqs);
static void handleUnzip(std::vector<Sequence<int>*>& seqs);
static void handleRemoveItem(std::vector<Sequence<int>*>& seqs);

void runUI() {
    std::vector<Sequence<int>*> seqs;
    bool running = true;

    while (running) {

        try {
            std::cout << "\n==== MENU ====\n"
                      << "1) Create sequence\n"
                      << "2) Append\n"
                      << "3) Print ALL\n"
                      << "4) Remove sequence\n"
                      << "5) Subsequence\n"
                      << "6) Concat\n"
                      << "7) zip\n"
                      << "8) Unzip\n"
                      << "9) Remove item\n"
                      << "0) Exit\n"
                      << "Choose: ";

            int cmd;
            std::cin >> cmd;
            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore(10000,'\n');
                throw MyException(ErrorType::InvalidArg, 0);
            }

            switch (cmd) {
            case 1:
                handleCreate(seqs);
                break;
            case 2:
                handleAppend(seqs);
                break;
            case 3:
                handlePrintAll(seqs);
                break;
            case 4:
                handleRemoveSequence(seqs);
                break;
            case 5:
                handleSubsequence(seqs);
                break;
            case 6:
                handleConcat(seqs);
                break;
            case 7:
                handleZip(seqs);
                break;
            case 8:
                handleUnzip(seqs);
                break;
            case 9:
                handleRemoveItem(seqs);
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "[Warn] Unknown command.\n";
            }
        }
        catch (const MyException &ex) {
            handleException(ex);
        }
    }

    for (auto *ptr : seqs) {
        delete ptr;
    }
    seqs.clear();
    std::cout << "Program finished.\n";
}

static void handleCreate(std::vector<Sequence<int>*>& seqs) {
    std::cout << "Choose type:\n"
              << " 1) ArraySequence\n"
              << " 2) ListSequence\n"
              << " 3) ImmutableArraySequence\n"
              << " 4) ImmutableListSequence\n"
              << "Enter: ";
    int choice;
    std::cin >> choice;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }

    Sequence<int>* newSeq = nullptr;
    switch (choice) {
    case 1: {
        newSeq = new ArraySequence<int>();
        std::cout << "[OK] Created ArraySequence, ID=" << seqs.size() << "\n";
        break;
    }
    case 2: {
        newSeq = new ListSequence<int>();
        std::cout << "[OK] Created ListSequence, ID=" << seqs.size() << "\n";
        break;
    }
    case 3: {
        std::cout << "Enter initial size for ImmutableArray: ";
        int n;
        std::cin >> n;
        if (!std::cin || n < 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            throw MyException(ErrorType::NegativeSize, 0);
        }
        int* arr = new int[n];
        for (int i = 0; i < n; i++) {
            std::cout << "arr[" << i << "]=";
            std::cin >> arr[i];
            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                delete[] arr;
                throw MyException(ErrorType::InvalidArg, 1);
            }
        }
        newSeq = new ImmutableArraySequence<int>(arr, n);
        delete[] arr;
        std::cout << "[OK] Created ImmutableArraySequence, ID=" << seqs.size() << "\n";
        break;
    }
    case 4: {
        std::cout << "Enter initial size for ImmutableList: ";
        int n;
        std::cin >> n;
        if (!std::cin || n < 0) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            throw MyException(ErrorType::NegativeSize, 0);
        }
        int* arr = new int[n];
        for (int i = 0; i < n; i++) {
            std::cout << "arr[" << i << "]=";
            std::cin >> arr[i];
            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                delete[] arr;
                throw MyException(ErrorType::InvalidArg, 1);
            }
        }
        newSeq = new ImmutableListSequence<int>(arr, n);
        delete[] arr;
        std::cout << "[OK] Created ImmutableListSequence, ID=" << seqs.size() << "\n";
        break;
    }
    default:
        throw MyException(ErrorType::OutOfRange, 3);
    }

    seqs.push_back(newSeq);
}

static void handleAppend(std::vector<Sequence<int>*>& seqs) {
    if (seqs.empty()) {
        std::cout << "[Warn] No sequences.\n";
        return;
    }
    std::cout << "Existing IDs: [0.." << (seqs.size()-1) << "]\n"
              << "Enter ID: ";
    int id;
    std::cin >> id;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    if (id < 0 || id >= (int)seqs.size()) {
        throw MyException(ErrorType::OutOfRange, 1);
    }

    std::cout << "Value to append: ";
    int val;
    std::cin >> val;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }

    seqs[id] = seqs[id]->Append(val);
    std::cout << "[OK] appended " << val << " to seq #" << id << "\n";
}

static void handlePrintAll(const std::vector<Sequence<int>*>& seqs) {
    if (seqs.empty()) {
        std::cout << "[Warn] No sequences.\n";
        return;
    }
    for (int i = 0; i < (int)seqs.size(); i++) {
        auto* seq = seqs[i];
        std::cout << "Seq #" << i << " (" << seq->TypeName() 
            << ", len=" << seq->GetLength() << "): ";
        for (int j = 0; j < seq->GetLength(); j++) {
            std::cout << seq->Get(j) << " ";
        }
        std::cout << "\n";
    }
}

static void handleRemoveSequence(std::vector<Sequence<int>*>& seqs) {
    if (seqs.empty()) {
        std::cout << "[Warn] No sequences.\n";
        return;
    }
    std::cout << "Which ID to remove? [0.." << (seqs.size()-1) << "]: ";
    int id;
    std::cin >> id;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    if (id < 0 || id >= (int)seqs.size()) {
        throw MyException(ErrorType::OutOfRange, 1);
    }

    delete seqs[id];
    seqs.erase(seqs.begin() + id);
    std::cout << "[OK] Removed seq #" << id << "\n";
}

static void handleSubsequence(std::vector<Sequence<int>*>& seqs) {
    if (seqs.empty()) {
        std::cout << "[Warn] No sequences.\n";
        return;
    }

    std::cout << "Available sequences:\n";
    for (int i = 0; i < (int)seqs.size(); i++) {
        std::cout << "  ID=" << i << ", size=" << seqs[i]->GetLength() << "\n";
    }

    std::cout << "Enter ID of sequence to extract from: ";
    int id;
    std::cin >> id;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    if (id < 0 || id >= (int)seqs.size()) {
        throw MyException(ErrorType::OutOfRange, 1);
    }

    int length = seqs[id]->GetLength();
    if (length == 0) {
        throw MyException(ErrorType::OutOfRange, 3);
    }

    std::cout << "Enter [start] and [end] indices (inclusive, 0 ≤ start ≤ end < "
              << length << "): ";

    int st, en;
    std::cin >> st >> en;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }

    Sequence<int>* sub = seqs[id]->GetSubsequence(st, en);
    seqs.push_back(sub);
    std::cout << "[OK] subsequence added as ID=" << (seqs.size() - 1) << "\n";
}


static void handleConcat(std::vector<Sequence<int>*>& seqs) {
    if (seqs.size() < 2) {
        std::cout << "[Warn] Need at least 2 sequences.\n";
        return;
    }
    std::cout << "Enter ID1, ID2: ";
    int id1, id2;
    std::cin >> id1 >> id2;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    if (id1 < 0 || id1 >= (int)seqs.size() || id2 < 0 || id2 >= (int)seqs.size()) {
        throw MyException(ErrorType::OutOfRange, 1);
    }
    Sequence<int>* result = seqs[id1]->Concat(seqs[id2]);
    seqs.push_back(result);
    std::cout << "[OK] concat => new seq ID=" << (seqs.size()-1) << "\n";
}

static void handleZip(std::vector<Sequence<int>*>& seqs) {
    if (seqs.size() < 2) {
        std::cout << "[Warn] Need at least 2 sequences.\n";
        return;
    }
    std::cout << "Enter ID1, ID2 for zip: ";
    int id1, id2;
    std::cin >> id1 >> id2;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    if (id1 < 0 || id1 >= (int)seqs.size() || id2 < 0 || id2 >= (int)seqs.size()) {
        throw MyException(ErrorType::OutOfRange, 1);
    }
    auto* zipped = zip<int,int>(seqs[id1], seqs[id2]);
    seqs.push_back((Sequence<int>*)zipped);
    std::cout << "[OK] zip => new seq of pairs, ID=" << (seqs.size()-1) << "\n";
}

static void handleUnzip(std::vector<Sequence<int>*>& seqs) {
    if (seqs.empty()) {
        std::cout << "[Warn] No sequences.\n";
        return;
    }
    std::cout << "Enter ID for unzip (should be seq of pairs): ";
    int id;
    std::cin >> id;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    if (id < 0 || id >= (int)seqs.size()) {
        throw MyException(ErrorType::OutOfRange, 1);
    }

    auto* seqPairs = (Sequence<std::pair<int,int>>*) (seqs[id]);
    auto res = unzip<int,int>(seqPairs);
    seqs.push_back(res.first);
    seqs.push_back(res.second);
    std::cout << "[OK] unzip => new IDs=" << (seqs.size()-2)
              << " and " << (seqs.size()-1) << "\n";
}

static void handleRemoveItem(std::vector<Sequence<int>*>& seqs) {
    if (seqs.empty()) {
        std::cout << "[Warn] No sequences.\n";
        return;
    }
    std::cout << "Which sequence ID to remove item from? [0.." << (seqs.size()-1) << "]: ";
    int id;
    std::cin >> id;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    if (id < 0 || id >= (int)seqs.size()) {
        throw MyException(ErrorType::OutOfRange, 1);
    }

    std::cout << "Index of element to remove: ";
    int idx;
    std::cin >> idx;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000,'\n');
        throw MyException(ErrorType::InvalidArg, 1);
    }
    seqs[id] = seqs[id]->RemoveAt(idx);
    std::cout << "[OK] Removed item at index " << idx << " in seq #" << id << "\n";
}