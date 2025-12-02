#include <iostream>
#include <cassert>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "ArraySequence.h"
#include "ListSequence.h"
#include "ImmutableArraySequence.h"
#include "ImmutableListSequence.h"
#include "Functions.h"

void TestDynamicArray() {
    int arr1[] = {1, 2, 3};
    DynamicArray<int> da(arr1, 3);
    assert(da.GetSize() == 3);
    assert(da.Get(0) == 1);
    assert(da.Get(1) == 2);
    assert(da.Get(2) == 3);

    DynamicArray<int> da2(5);
    assert(da2.GetSize() == 5);
    da2.Set(0, 10);
    da2.Set(1, 20);
    assert(da2.Get(0) == 10);
    assert(da2.Get(1) == 20);

    DynamicArray<int> da3(da);
    assert(da3.GetSize() == 3);
    assert(da3.Get(0) == 1);
    assert(da3.Get(1) == 2);

    DynamicArray<int> da4(3);
    da4[0] = 100;
    da4[1] = 200;
    assert(da4[0] == 100);
    assert(da4[1] == 200);

    int arr2[] = {1, 2, 3, 4, 5};
    DynamicArray<int> da5(arr2, 5);
    da5.Resize(3);
    assert(da5.GetSize() == 3);
    assert(da5.Get(0) == 1);
    assert(da5.Get(1) == 2);

    std::cout << "DynamicArray tests passed!\n";
}

void TestLinkedList() {
    LinkedList<int> list;
    assert(list.GetLength() == 0);
    list.Append(1);
    list.Append(2);
    assert(list.GetLength() == 2);
    assert(list.GetFirst() == 1);
    assert(list.GetLast() == 2);

    int arr[] = {1, 2, 3};
    LinkedList<int> list2(arr, 3);
    assert(list2.GetLength() == 3);
    assert(list2.Get(0) == 1);
    assert(list2.Get(2) == 3);

    LinkedList<int> list3(list2);
    assert(list3.GetLength() == 3);
    assert(list3.Get(1) == 2);

    list3.Prepend(0);
    assert(list3.GetLength() == 4);
    assert(list3.GetFirst() == 0);

    list3.InsertAt(99, 2);
    assert(list3.GetLength() == 5);
    assert(list3.Get(2) == 99);

    list3.RemoveAt(2);
    assert(list3.GetLength() == 4);
    assert(list3.Get(2) == 2);

    list3.reverse();
    assert(list3.GetFirst() == 3);
    assert(list3.GetLast() == 0);

    std::cout << "LinkedList tests passed!\n";
}

void TestArraySequence() {
    ArraySequence<int> seq;
    assert(seq.GetLength() == 0);
    seq.Append(1);
    seq.Append(2);
    assert(seq.GetLength() == 2);
    assert(seq.GetFirst() == 1);
    assert(seq.GetLast() == 2);

    int arr1[] = {1, 2, 3, 4, 5};
    ArraySequence<int> seq2(arr1, 5);
    assert(seq2.GetLength() == 5);
    assert(seq2.Get(0) == 1);
    assert(seq2.Get(4) == 5);

    ArraySequence<int> seq3;
    seq3.Append(1);
    seq3.Append(2);
    seq3.Prepend(0);
    assert(seq3.GetLength() == 3);
    assert(seq3.GetFirst() == 0);
    assert(seq3.Get(1) == 1);

    ArraySequence<int> seq4;
    seq4.Append(1);
    seq4.Append(3);
    seq4.InsertAt(2, 1);
    assert(seq4.GetLength() == 3);
    assert(seq4.Get(0) == 1);
    assert(seq4.Get(1) == 2);
    assert(seq4.Get(2) == 3);

    int arr2[] = {1, 2, 3, 4, 5};
    ArraySequence<int> seq5(arr2, 5);
    Sequence<int>* sub = seq5.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 2);
    assert(sub->Get(1) == 3);
    assert(sub->Get(2) == 4);
    delete sub;

    int arr3[] = {1, 2};
    int arr4[] = {3, 4};
    ArraySequence<int> seq6(arr3, 2);
    ArraySequence<int> seq7(arr4, 2);
    Sequence<int>* result = seq6.Concat(&seq7);
    assert(result->GetLength() == 4);
    assert(result->Get(0) == 1);
    assert(result->Get(3) == 4);
    delete result;

    ArraySequence<int> seq8;
    seq8.Append(1);
    seq8.Append(2);
    seq8.Append(3);
    seq8.RemoveAt(1);
    assert(seq8.GetLength() == 2);
    assert(seq8.Get(0) == 1);
    assert(seq8.Get(1) == 3);

    ArraySequence<int> seq9;
    for (int i = 0; i < 10; i++) {
        seq9.Append(i);
    }
    assert(seq9.GetLength() == 10);
    assert(seq9.Get(9) == 9);

    std::cout << "ArraySequence tests passed!\n";
}

void TestListSequence() {
    ListSequence<int> seq;
    assert(seq.GetLength() == 0);
    seq.Append(1);
    seq.Append(2);
    assert(seq.GetLength() == 2);
    assert(seq.GetFirst() == 1);
    assert(seq.GetLast() == 2);

    int arr1[] = {1, 2, 3, 4, 5};
    ListSequence<int> seq2(arr1, 5);
    assert(seq2.GetLength() == 5);
    assert(seq2.Get(0) == 1);
    assert(seq2.Get(4) == 5);

    ListSequence<int> seq3;
    seq3.Append(1);
    seq3.Append(2);
    seq3.Prepend(0);
    assert(seq3.GetLength() == 3);
    assert(seq3.GetFirst() == 0);
    assert(seq3.Get(1) == 1);

    ListSequence<int> seq4;
    seq4.Append(1);
    seq4.Append(3);
    seq4.InsertAt(2, 1);
    assert(seq4.GetLength() == 3);
    assert(seq4.Get(0) == 1);
    assert(seq4.Get(1) == 2);
    assert(seq4.Get(2) == 3);

    int arr2[] = {1, 2, 3, 4, 5};
    ListSequence<int> seq5(arr2, 5);
    Sequence<int>* sub = seq5.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 2);
    assert(sub->Get(1) == 3);
    assert(sub->Get(2) == 4);
    delete sub;

    std::cout << "ListSequence tests passed!\n";
}

void TestImmutableArraySequence() {
    ImmutableArraySequence<int> seq;
    assert(seq.GetLength() == 0);
    assert(seq.IsEmpty());

    int arr[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> seq2(arr, 5);
    assert(seq2.GetLength() == 5);
    assert(seq2.GetFirst() == 1);
    assert(seq2.GetLast() == 5);
    assert(seq2.Get(2) == 3);

    ImmutableArraySequence<int> original(arr, 3);
    Sequence<int>* newSeq = original.Append(4);
    assert(original.GetLength() == 3);
    assert(original.GetLast() == 3);
    assert(newSeq->GetLength() == 4);
    assert(newSeq->GetLast() == 4);
    delete newSeq;

    ImmutableArraySequence<int> original2(arr, 3);
    Sequence<int>* newSeq2 = original2.Prepend(0);
    assert(original2.GetLength() == 3);
    assert(original2.GetFirst() == 1);
    assert(newSeq2->GetLength() == 4);
    assert(newSeq2->GetFirst() == 0);
    delete newSeq2;

    int arr3[] = {1, 3, 4};
    ImmutableArraySequence<int> original3(arr3, 3);
    Sequence<int>* newSeq3 = original3.InsertAt(2, 1);
    assert(original3.GetLength() == 3);
    assert(original3.Get(1) == 3);
    assert(newSeq3->GetLength() == 4);
    assert(newSeq3->Get(1) == 2);
    delete newSeq3;

    int arr4[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> original4(arr4, 4);
    Sequence<int>* newSeq4 = original4.RemoveAt(1);
    assert(original4.GetLength() == 4);
    assert(original4.Get(1) == 2);
    assert(newSeq4->GetLength() == 3);
    assert(newSeq4->Get(1) == 3);
    delete newSeq4;

    int arr5[] = {1, 2};
    int arr6[] = {3, 4};
    ImmutableArraySequence<int> seq5(arr5, 2);
    ImmutableArraySequence<int> seq6(arr6, 2);
    Sequence<int>* concatenated = seq5.Concat(&seq6);
    assert(seq5.GetLength() == 2);
    assert(seq6.GetLength() == 2);
    assert(concatenated->GetLength() == 4);
    assert(concatenated->Get(0) == 1);
    assert(concatenated->Get(3) == 4);
    delete concatenated;

    int arr7[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> original5(arr7, 5);
    Sequence<int>* sub = original5.GetSubsequence(1, 3);
    assert(original5.GetLength() == 5);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 2);
    assert(sub->Get(2) == 4);
    delete sub;

    std::cout << "ImmutableArraySequence tests passed!\n";
}

void TestImmutableListSequence() {
    ImmutableListSequence<int> seq;
    assert(seq.GetLength() == 0);
    assert(seq.IsEmpty());

    int arr[] = {1, 2, 3, 4, 5};
    ImmutableListSequence<int> seq2(arr, 5);
    assert(seq2.GetLength() == 5);
    assert(seq2.GetFirst() == 1);
    assert(seq2.GetLast() == 5);
    assert(seq2.Get(2) == 3);

    ImmutableListSequence<int> original(arr, 3);
    Sequence<int>* newSeq = original.Append(4);
    assert(original.GetLength() == 3);
    assert(original.GetLast() == 3);
    assert(newSeq->GetLength() == 4);
    assert(newSeq->GetLast() == 4);
    delete newSeq;

    int arr2[] = {1, 2, 3, 4, 5};
    ImmutableListSequence<int> original2(arr2, 5);
    Sequence<int>* sub = original2.GetSubsequence(1, 3);
    assert(original2.GetLength() == 5);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 2);
    assert(sub->Get(2) == 4);
    delete sub;

    std::cout << "ImmutableListSequence tests passed!\n";
}

void TestZipUnzip() {
    ArraySequence<int> seq1;
    ArraySequence<int> seq2;
    
    seq1.Append(1);
    seq1.Append(2);
    seq1.Append(3);
    
    seq2.Append(4);
    seq2.Append(5);
    seq2.Append(6);
    
    auto* zipped = zip(&seq1, &seq2);
    assert(zipped->GetLength() == 3);
    
    auto pair1 = zipped->Get(0);
    assert(pair1.first == 1);
    assert(pair1.second == 4);
    
    auto pair2 = zipped->Get(1);
    assert(pair2.first == 2);
    assert(pair2.second == 5);
    
    auto result = unzip(zipped);
    assert(result.first->GetLength() == 3);
    assert(result.second->GetLength() == 3);
    
    assert(result.first->Get(0) == 1);
    assert(result.second->Get(0) == 4);
    
    delete zipped;
    delete result.first;
    delete result.second;
    
    std::cout << "Zip/Unzip tests passed!\n";
}

int main() {
    std::cout << "Running tests...\n";

    TestDynamicArray();
    TestLinkedList();
    TestArraySequence();
    TestListSequence();
    TestImmutableArraySequence();
    TestImmutableListSequence();
    TestZipUnzip();

    std::cout << "All tests passed successfully!\n";
    return 0;
}