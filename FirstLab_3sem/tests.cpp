#include <iostream>
#include <cassert>
#include <string>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "Functions.h"

template<class Seq>
void checkEqual(const Seq& seq, std::initializer_list<int> ref)
{
    assert(seq.GetLength() == static_cast<int>(ref.size()));
    int i = 0;
    for (int v : ref) { assert(seq.Get(i) == v); ++i; }
}
void TestDynamicArray() {
    std::cout << "Testing DynamicArray...\n";
    
    {
        int arr[] = {1, 2, 3};
        DynamicArray<int> da(arr, 3);
        assert(da.GetSize() == 3);
        assert(da.Get(0) == 1);
        assert(da.Get(1) == 2);
        assert(da.Get(2) == 3);
    }
    
    {
        DynamicArray<int> da(5);
        assert(da.GetSize() == 5);
        da.Set(0, 10);
        da.Set(1, 20);
        assert(da.Get(0) == 10);
        assert(da.Get(1) == 20);
    }
    
    {
        int arr[] = {1, 2, 3};
        DynamicArray<int> da1(arr, 3);
        DynamicArray<int> da2(da1);
        assert(da2.GetSize() == 3);
        assert(da2.Get(0) == 1);
        assert(da2.Get(1) == 2);
    }
    
    {
        DynamicArray<int> da(3);
        da[0] = 100;
        da[1] = 200;
        assert(da[0] == 100);
        assert(da[1] == 200);
    }
    
    {
        int arr[] = {1, 2, 3, 4, 5};
        DynamicArray<int> da(arr, 5);
        da.Resize(3);
        assert(da.GetSize() == 3);
        assert(da.Get(0) == 1);
        assert(da.Get(1) == 2);
    }
    
    std::cout << "DynamicArray tests passed!\n\n";
}

void TestArraySequence() {
    std::cout << "Testing ArraySequence...\n";
    
    {
        ArraySequence<int> seq;
        assert(seq.GetLength() == 0);
        seq.Append(1);
        seq.Append(2);
        assert(seq.GetLength() == 2);
        assert(seq.GetFirst() == 1);
        assert(seq.GetLast() == 2);
    }
    
    {
        int arr[] = {1, 2, 3, 4, 5};
        ArraySequence<int> seq(arr, 5);
        assert(seq.GetLength() == 5);
        assert(seq.Get(0) == 1);
        assert(seq.Get(4) == 5);
    }
    
    {
        ArraySequence<int> seq;
        seq.Append(1);
        seq.Append(2);
        seq.Prepend(0);
        assert(seq.GetLength() == 3);
        assert(seq.GetFirst() == 0);
        assert(seq.Get(1) == 1);
    }
    
    {
        ArraySequence<int> seq;
        seq.Append(1);
        seq.Append(3);
        seq.InsertAt(2, 1);
        assert(seq.GetLength() == 3);
        assert(seq.Get(0) == 1);
        assert(seq.Get(1) == 2);
        assert(seq.Get(2) == 3);
    }
    
    {
        int arr[] = {1, 2, 3, 4, 5};
        ArraySequence<int> seq(arr, 5);
        Sequence<int>* sub = seq.GetSubsequence(1, 3);
        assert(sub->GetLength() == 3);
        assert(sub->Get(0) == 2);
        assert(sub->Get(1) == 3);
        assert(sub->Get(2) == 4);
        delete sub;
    }
    
    {
        int arr1[] = {1, 2};
        int arr2[] = {3, 4};
        ArraySequence<int> seq1(arr1, 2);
        ArraySequence<int> seq2(arr2, 2);
        Sequence<int>* result = seq1.Concat(&seq2);
        assert(result->GetLength() == 4);
        assert(result->Get(0) == 1);
        assert(result->Get(3) == 4);
        delete result;
    }
    
    {
        ArraySequence<int> seq;
        seq.Append(1);
        seq.Append(2);
        seq.Append(3);
        seq.RemoveAt(1);
        assert(seq.GetLength() == 2);
        assert(seq.Get(0) == 1);
        assert(seq.Get(1) == 3);
    }
    
    {
        ArraySequence<int> seq;
        for (int i = 0; i < 10; i++) {
            seq.Append(i);
        }
        assert(seq.GetLength() == 10);
        assert(seq.Get(9) == 9);
    }
    
    std::cout << "ArraySequence tests passed!\n\n";
}

void TestListSequence() {
    std::cout << "Testing ListSequence...\n";
    
    {
        ListSequence<int> seq;
        assert(seq.GetLength() == 0);
        seq.Append(1);
        seq.Append(2);
        assert(seq.GetLength() == 2);
        assert(seq.GetFirst() == 1);
        assert(seq.GetLast() == 2);
    }
    
    {
        int arr[] = {1, 2, 3, 4, 5};
        ListSequence<int> seq(arr, 5);
        assert(seq.GetLength() == 5);
        assert(seq.Get(0) == 1);
        assert(seq.Get(4) == 5);
    }
    
    {
        ListSequence<int> seq;
        seq.Append(1);
        seq.Append(2);
        seq.Prepend(0);
        assert(seq.GetLength() == 3);
        assert(seq.GetFirst() == 0);
        assert(seq.Get(1) == 1);
    }
    
    {
        ListSequence<int> seq;
        seq.Append(1);
        seq.Append(3);
        seq.InsertAt(2, 1);
        assert(seq.GetLength() == 3);
        assert(seq.Get(0) == 1);
        assert(seq.Get(1) == 2);
        assert(seq.Get(2) == 3);
    }
    
    {
        int arr[] = {1, 2, 3, 4, 5};
        ListSequence<int> seq(arr, 5);
        Sequence<int>* sub = seq.GetSubsequence(1, 3);
        assert(sub->GetLength() == 3);
        assert(sub->Get(0) == 2);
        assert(sub->Get(1) == 3);
        assert(sub->Get(2) == 4);
        delete sub;
    }
    
    std::cout << "ListSequence tests passed!\n\n";
}

void TestSequenceComparison() {
    std::cout << "Testing Sequence Comparison...\n";
    
    {
        ArraySequence<int> seq1;
        ArraySequence<int> seq2;
        seq1.Append(1);
        seq1.Append(2);
        seq2.Append(1);
        seq2.Append(2);
        assert(seq1 == seq2);
        
        seq2.Append(3);
        assert(seq1 != seq2);
    }
    
    {
        ListSequence<int> seq1;
        ListSequence<int> seq2;
        seq1.Append(1);
        seq1.Append(2);
        seq2.Append(1);
        seq2.Append(2);
        assert(seq1 == seq2);
        
        seq2.Append(3);
        assert(seq1 != seq2);
    }
    
    std::cout << "Sequence Comparison tests passed!\n\n";
}

void TestManyRepeats()
{
    const int N = 10000;
    ListSequence<int> s;
    for (int i = 0; i < N; ++i){
        s.Append(7);
    }
    assert(s.GetLength() == N);
    s.reverse();
    ListSequence<int> ref;
    for (int i = 0; i < N; ++i){
        ref.Append(7);
    }
    assert(s == ref);
}

template<class Seq>
void TestReverseTwice()
{
    Seq s;
    for (int v : {1,2,3,4,5}) {
        s.Append(v);
    }
    auto orig = s.Clone();
    s.reverse();
    s.reverse();
    assert(s == *orig);
    delete orig;
}

void TestCycleSmartReverse()
{
    {
        LinkedList<int> cyc;
        for (int v : {1,2,3,4}) cyc.Append(v);
        cyc.makeCycle();
        cyc.smartReverse();
        std::vector<int> got;
        int& cur = cyc.GetFirst();
        got.push_back(cur);
        for (int i = 0; i < 7; ++i){
            got.push_back(cyc.Next(got.back()));
        }
        std::vector<int> exp{4,3,2,1,4,3,2,1};
        assert(got == exp);
    }

    {
        LinkedList<int> cyc;
        for (int v : {1,2,3,4}) cyc.Append(v);
        cyc.MakeCycle(1);
        cyc.smartReverse();
        std::vector<int> got;
        int& cur = cyc.GetFirst();
        got.push_back(cur);
        for (int i = 0; i < 7; ++i){
            got.push_back(cyc.Next(got.back()));
        }
        std::vector<int> exp{4,3,2,1,4,3,2,1};
        assert(got == exp);
    }
}

void TestImmutableArraySequence() {
    std::cout << "Testing ImmutableArraySequence...\n";
    
    // Тест 1: Создание пустой последовательности
    {
        ImmutableArraySequence<int> seq;
        assert(seq.GetLength() == 0);
        assert(seq.IsEmpty());
        std::cout << "Test 1 passed: Empty sequence\n";
    }
    
    // Тест 2: Создание из массива
    {
        int arr[] = {1, 2, 3, 4, 5};
        ImmutableArraySequence<int> seq(arr, 5);
        assert(seq.GetLength() == 5);
        assert(seq.GetFirst() == 1);
        assert(seq.GetLast() == 5);
        assert(seq.Get(2) == 3);
        std::cout << "Test 2 passed: Creation from array\n";
    }
    
    // Тест 3: Иммутабельность Append
    {
        int arr[] = {1, 2, 3};
        ImmutableArraySequence<int> original(arr, 3);
        Sequence<int>* newSeq = original.Append(4);
        
        // Проверяем, что оригинал не изменился
        assert(original.GetLength() == 3);
        assert(original.GetLast() == 3);
        
        // Проверяем новую последовательность
        assert(newSeq->GetLength() == 4);
        assert(newSeq->GetLast() == 4);
        
        delete newSeq;
        std::cout << "Test 3 passed: Immutable Append\n";
    }
    
    // Тест 4: Иммутабельность Prepend
    {
        int arr[] = {2, 3, 4};
        ImmutableArraySequence<int> original(arr, 3);
        Sequence<int>* newSeq = original.Prepend(1);
        
        // Проверяем, что оригинал не изменился
        assert(original.GetLength() == 3);
        assert(original.GetFirst() == 2);
        
        // Проверяем новую последовательность
        assert(newSeq->GetLength() == 4);
        assert(newSeq->GetFirst() == 1);
        
        delete newSeq;
        std::cout << "Test 4 passed: Immutable Prepend\n";
    }
    
    // Тест 5: Иммутабельность InsertAt
    {
        int arr[] = {1, 3, 4};
        ImmutableArraySequence<int> original(arr, 3);
        Sequence<int>* newSeq = original.InsertAt(2, 1);
        
        // Проверяем, что оригинал не изменился
        assert(original.GetLength() == 3);
        assert(original.Get(1) == 3);
        
        // Проверяем новую последовательность
        assert(newSeq->GetLength() == 4);
        assert(newSeq->Get(1) == 2);
        
        delete newSeq;
        std::cout << "Test 5 passed: Immutable InsertAt\n";
    }
    
    // Тест 6: Иммутабельность RemoveAt
    {
        int arr[] = {1, 2, 3, 4};
        ImmutableArraySequence<int> original(arr, 4);
        Sequence<int>* newSeq = original.RemoveAt(1);
        
        // Проверяем, что оригинал не изменился
        assert(original.GetLength() == 4);
        assert(original.Get(1) == 2);
        
        // Проверяем новую последовательность
        assert(newSeq->GetLength() == 3);
        assert(newSeq->Get(1) == 3);
        
        delete newSeq;
        std::cout << "Test 6 passed: Immutable RemoveAt\n";
    }
    
    // Тест 7: Иммутабельность Concat
    {
        int arr1[] = {1, 2};
        int arr2[] = {3, 4};
        ImmutableArraySequence<int> seq1(arr1, 2);
        ImmutableArraySequence<int> seq2(arr2, 2);
        
        Sequence<int>* concatenated = seq1.Concat(&seq2);
        
        // Проверяем, что оригиналы не изменились
        assert(seq1.GetLength() == 2);
        assert(seq2.GetLength() == 2);
        
        // Проверяем новую последовательность
        assert(concatenated->GetLength() == 4);
        assert(concatenated->Get(0) == 1);
        assert(concatenated->Get(3) == 4);
        
        delete concatenated;
        std::cout << "Test 7 passed: Immutable Concat\n";
    }
    
    // Тест 8: GetSubsequence
    {
        int arr[] = {1, 2, 3, 4, 5};
        ImmutableArraySequence<int> original(arr, 5);
        Sequence<int>* sub = original.GetSubsequence(1, 3);
        
        // Проверяем, что оригинал не изменился
        assert(original.GetLength() == 5);
        
        // Проверяем подпоследовательность
        assert(sub->GetLength() == 3);
        assert(sub->Get(0) == 2);
        assert(sub->Get(2) == 4);
        
        delete sub;
        std::cout << "Test 8 passed: GetSubsequence\n";
    }
    
    // Тест 9: Копирование и присваивание
    {
        int arr[] = {1, 2, 3};
        ImmutableArraySequence<int> original(arr, 3);
        ImmutableArraySequence<int> copy(original);
        ImmutableArraySequence<int> assigned;
        assigned = original;
        
        assert(copy.GetLength() == 3);
        assert(assigned.GetLength() == 3);
        assert(copy.Get(1) == 2);
        assert(assigned.Get(1) == 2);
        
        // Проверяем, что это глубокие копии
        Sequence<int>* modified = original.Append(4);
        assert(original.GetLength() == 3); // Оригинал не изменился
        assert(copy.GetLength() == 3);     // Копия не изменилась
        assert(assigned.GetLength() == 3); // Присвоенная не изменилась
        
        delete modified;
        std::cout << "Test 9 passed: Copy and assignment\n";
    }
    
    std::cout << "All ImmutableArraySequence tests passed!\n\n";
}

void TestImmutableListSequence() {
    std::cout << "Testing ImmutableListSequence...\n";
    
    // Тест 1: Создание пустой последовательности
    {
        ImmutableListSequence<int> seq;
        assert(seq.GetLength() == 0);
        assert(seq.IsEmpty());
        std::cout << "Test 1 passed: Empty sequence\n";
    }
    
    // Тест 2: Создание из массива
    {
        int arr[] = {1, 2, 3, 4, 5};
        ImmutableListSequence<int> seq(arr, 5);
        assert(seq.GetLength() == 5);
        assert(seq.GetFirst() == 1);
        assert(seq.GetLast() == 5);
        assert(seq.Get(2) == 3);
        std::cout << "Test 2 passed: Creation from array\n";
    }
    
    // Тест 3: Иммутабельность Append
    {
        int arr[] = {1, 2, 3};
        ImmutableListSequence<int> original(arr, 3);
        Sequence<int>* newSeq = original.Append(4);
        
        // Проверяем, что оригинал не изменился
        assert(original.GetLength() == 3);
        assert(original.GetLast() == 3);
        
        // Проверяем новую последовательность
        assert(newSeq->GetLength() == 4);
        assert(newSeq->GetLast() == 4);
        
        delete newSeq;
        std::cout << "Test 3 passed: Immutable Append\n";
    }
    
    // Тест 4: GetSubsequence
    {
        int arr[] = {1, 2, 3, 4, 5};
        ImmutableListSequence<int> original(arr, 5);
        Sequence<int>* sub = original.GetSubsequence(1, 3);
        
        // Проверяем, что оригинал не изменился
        assert(original.GetLength() == 5);
        
        // Проверяем подпоследовательность
        assert(sub->GetLength() == 3);
        assert(sub->Get(0) == 2);
        assert(sub->Get(2) == 4);
        
        delete sub;
        std::cout << "Test 4 passed: GetSubsequence\n";
    }
    
    std::cout << "All ImmutableListSequence tests passed!\n\n";
}

void TestMonadTuple2() {
    std::cout << "Testing MonadTuple2...\n";
    
    MonadTuple2<int, double> tuple(42, 3.14);
    assert(tuple.first == 42);
    assert(tuple.second == 3.14);
    
    auto mapped = tuple.Map([](double x) { return x * 2; });
    assert(mapped.first == 42);
    assert(mapped.second == 6.28);
    
    std::cout << "MonadTuple2 tests passed!\n";
}

void TestMonadTuple3() {
    std::cout << "Testing MonadTuple3...\n";
    
    MonadTuple3<int, double, char> tuple(1, 2.5, 'A');
    assert(tuple.first == 1);
    assert(tuple.second == 2.5);
    assert(tuple.third == 'A');
    
    std::cout << "MonadTuple3 tests passed!\n";
}

void TestZipAsTuple() {
    std::cout << "Testing zip_as_tuple...\n";
    
    ArraySequence<int> seq1;
    ArraySequence<double> seq2;
    
    seq1.Append(1);
    seq1.Append(2);
    seq1.Append(3);
    
    seq2.Append(1.1);
    seq2.Append(2.2);
    seq2.Append(3.3);
    
    auto* zipped = zip_as_tuple(&seq1, &seq2);
    assert(zipped->GetLength() == 3);
    
    auto tuple1 = zipped->Get(0);
    assert(tuple1.first == 1);
    assert(tuple1.second == 1.1);
    
    auto tuple2 = zipped->Get(1);
    assert(tuple2.first == 2);
    assert(tuple2.second == 2.2);
    
    delete zipped;
    std::cout << "zip_as_tuple tests passed!\n";
}

void TestUnzipTuple() {
    std::cout << "Testing unzip_tuple...\n";
    
    ArraySequence<MonadTuple2<int, double>> tuples;
    tuples.Append(MonadTuple2<int, double>(1, 1.1));
    tuples.Append(MonadTuple2<int, double>(2, 2.2));
    tuples.Append(MonadTuple2<int, double>(3, 3.3));
    
    auto result = unzip_tuple(&tuples);
    assert(result.first->GetLength() == 3);
    assert(result.second->GetLength() == 3);
    
    assert(result.first->Get(0) == 1);
    assert(result.second->Get(0) == 1.1);
    
    assert(result.first->Get(1) == 2);
    assert(result.second->Get(1) == 2.2);
    
    delete result.first;
    delete result.second;
    std::cout << "unzip_tuple tests passed!\n";
}

int main() {
    std::cout<<"Running tests...\n";

    TestDynamicArray();
    TestArraySequence();
    TestListSequence();
    TestSequenceComparison();
    TestManyRepeats();
    TestReverseTwice<ArraySequence<int>>();
    TestReverseTwice<ListSequence<int>>();
    TestCycleSmartReverse();

    TestImmutableArraySequence();
    TestImmutableListSequence();

    TestMonadTuple2();
    TestMonadTuple3();
    TestZipAsTuple();
    TestUnzipTuple();
    std::cout<<"All tests passed successfully!\n";
    return 0;
}