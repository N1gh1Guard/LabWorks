#include <iostream>
#include <cassert>
#include <vector>
#include <string>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "ArraySequence.h"
#include "ListSequence.h"

template<class Seq>
void checkEqual(const Seq& seq, std::initializer_list<int> ref)
{
    assert(seq.GetLength() == static_cast<int>(ref.size()));
    int i = 0;
    for (int v : ref) { assert(seq.Get(i) == v); ++i; }
}

void TestDynamicArray()
{
    int arr[]{1,2,3};
    DynamicArray<int> d(arr,3);
    assert(d.GetSize()==3);
    assert(d.Get(0)==1 && d.Get(1)==2 && d.Get(2)==3);

    d.Resize(5);
    d.Set(3,10); d.Set(4,20);
    assert(d.GetSize()==5);
    assert(d.Get(3)==10 && d.Get(4)==20);
}

void TestLinkedList()
{
    LinkedList<int> lst; lst.Append(10); lst.Append(20); lst.Prepend(5);
    assert(lst.GetLength()==3);
    assert(lst.Get(0)==5 && lst.Get(1)==10 && lst.Get(2)==20);

    auto* sub = lst.GetSubList(0,1);
    checkEqual(*sub,{5,10});
    delete sub;
}

void TestArraySequence()
{
    ArraySequence<int> s; s.Append(1)->Append(2)->Append(3);
    assert(s.GetFirst()==1 && s.GetLast()==3);

    s.Prepend(0);
    s.InsertAt(999,2);
    Sequence<int>* sub = s.GetSubsequence(1,3);
    checkEqual(*sub,{1,999,2});
    delete sub;
}

void TestListSequence()
{
    ListSequence<int> s; s.Append(1)->Append(2)->Append(3);
    assert(s.GetFirst()==1 && s.GetLast()==3);

    s.Prepend(0);
    s.InsertAt(999,2);
    auto* sub = s.GetSubsequence(1,3);
    checkEqual(*sub,{1,999,2});
    delete sub;
}

template<class Seq>
void ReverseScenarios(const char* tag)
{
    std::cout<<"  Reverse‑block for "<<tag<<'\n';

    { Seq s; s.reverse(); checkEqual(s,{}); }

    { Seq s; s.Append(42); s.reverse(); checkEqual(s,{42}); }

    { Seq s; s.Append(7)->Append(9); s.reverse(); checkEqual(s,{9,7});}

    { 
        Seq s; 
        for(int v:{1,2,3,4,5}) {
            s.Append(v);
        }
        s.reverse();
        checkEqual(s,{5,4,3,2,1}); 
    }
    
    {
        Seq s;
        for(int v:{10,20,30,40}) {
            s.Append(v);
        }
        s.reverse();
        checkEqual(s,{40,30,20,10});
    }
}



void TestOtherTypes()
{
    ArraySequence<double> ad; ad.Append(1.5)->Append(2.5);
    assert(ad.GetLast()==2.5);

    ListSequence<std::string> ls;
    ls.Append("a")->Append("b")->Prepend("z");
    assert(ls.GetFirst()=="z" && ls.GetLast()=="b");
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

int main()
{
    std::cout<<"Running tests...\n";

    TestDynamicArray();
    TestLinkedList();
    TestArraySequence();
    TestListSequence();
    ReverseScenarios<ListSequence<int>>("ListSequence");

    TestOtherTypes();
    TestManyRepeats();
    TestReverseTwice<ArraySequence<int>>();
    TestReverseTwice<ListSequence<int>>();
    TestCycleSmartReverse();

    std::cout<<"All tests passed successfully!\n";
    return 0;
}