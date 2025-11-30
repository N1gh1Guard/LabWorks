#pragma once

template<typename T1, typename T2>
struct Pair {
    T1 first;
    T2 second;
    Pair(T1 f, T2 s) : first(f), second(s) {}
};

template<typename T1, typename T2, typename T3>
struct Triple {
    T1 first;
    T2 second;
    T3 third;
    Triple(T1 f, T2 s, T3 t) : first(f), second(s), third(t) {}
};

template<typename T1, typename T2, typename T3, typename T4>
struct Quadruple {
    T1 first;
    T2 second;
    T3 third;
    T4 fourth;
    Quadruple(T1 f, T2 s, T3 t, T4 fo) : first(f), second(s), third(t), fourth(fo) {}
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
struct Quintuple {
    T1 first;
    T2 second;
    T3 third;
    T4 fourth;
    T5 fifth;
    Quintuple(T1 f, T2 s, T3 t, T4 fo, T5 fi) : first(f), second(s), third(t), fourth(fo), fifth(fi) {}
};