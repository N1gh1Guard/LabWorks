#pragma once

template<typename T1, typename T2>
class MonadTuple2 {
public:
    T1 first;
    T2 second;
    
    MonadTuple2(T1 a, T2 b) : first(a), second(b) {}
    
    template <typename F>
    auto Bind(F f) const -> decltype(f(second)) {
        return f(second);
    }
    
    template <typename F>
    auto Map(F f) const -> MonadTuple2<T1, decltype(f(second))> {
        return MonadTuple2<T1, decltype(f(second))>(first, f(second));
    }
    
    template <typename F>
    auto AndThen(F f) const -> decltype(f(second)) {
        return Bind(f);
    }
};

template<typename T1, typename T2, typename T3>
class MonadTuple3 {
public:
    T1 first;
    T2 second;
    T3 third;
    
    MonadTuple3(T1 a, T2 b, T3 c) : first(a), second(b), third(c) {}
    
    template <typename F>
    auto Bind(F f) const -> decltype(f(third)) {
        return f(third);
    }
    
    template <typename F>
    auto Map(F f) const -> MonadTuple3<T1, T2, decltype(f(third))> {
        return MonadTuple3<T1, T2, decltype(f(third))>(first, second, f(third));
    }
};

template<typename T1, typename T2, typename T3, typename T4>
class MonadTuple4 {
public:
    T1 first;
    T2 second;
    T3 third;
    T4 fourth;
    
    MonadTuple4(T1 a, T2 b, T3 c, T4 d) : first(a), second(b), third(c), fourth(d) {}
    
    template <typename F>
    auto Bind(F f) const -> decltype(f(fourth)) {
        return f(fourth);
    }
    
    template <typename F>
    auto Map(F f) const -> MonadTuple4<T1, T2, T3, decltype(f(fourth))> {
        return MonadTuple4<T1, T2, T3, decltype(f(fourth))>(first, second, third, f(fourth));
    }
};

template<typename T1, typename T2, typename T3, typename T4, typename T5>
class MonadTuple5 {
public:
    T1 first;
    T2 second;
    T3 third;
    T4 fourth;
    T5 fifth;
    
    MonadTuple5(T1 a, T2 b, T3 c, T4 d, T5 e) : first(a), second(b), third(c), fourth(d), fifth(e) {}
    
    template <typename F>
    auto Bind(F f) const -> decltype(f(fifth)) {
        return f(fifth);
    }
    
    template <typename F>
    auto Map(F f) const -> MonadTuple5<T1, T2, T3, T4, decltype(f(fifth))> {
        return MonadTuple5<T1, T2, T3, T4, decltype(f(fifth))>(first, second, third, fourth, f(fifth));
    }
};