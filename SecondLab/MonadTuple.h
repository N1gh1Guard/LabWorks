#pragma once
#include <tuple>
#include <utility>
#include <functional>
#include <vector>
#include <memory>
#include "Option.h"

// Базовый класс для полиморфизма
class TupleBase {
public:
    virtual size_t size() const = 0;
    virtual void* get_element(size_t index) const = 0;
    virtual ~TupleBase() = default;
};

// Реализация кортежа произвольного размера и типов
template <typename... Ts>
class MonadTuple : public TupleBase {
private:
    std::tuple<Ts...> values;
    
public:
    MonadTuple(Ts... vs) : values(vs...) {}
    
    size_t size() const override {
        return sizeof...(Ts);
    }
    
    void* get_element(size_t index) const override {
        void* result = nullptr;
        size_t current = 0;
        
        auto extractor = [&](auto&&... args) {
            ((current++ == index ? (result = static_cast<void*>(&args), 0 : 0), ...);
        };
        
        std::apply(extractor, values);
        return result;
    }
    
    // Получение значения по индексу с сохранением типа
    template <size_t I>
    auto get() const -> decltype(auto) {
        return std::get<I>(values);
    }
    
    // Преобразование в std::tuple
    auto to_std_tuple() const {
        return values;
    }
    
    // Применение функции к элементам
    template <typename F>
    void apply(F&& f) const {
        std::apply([&](auto&&... args) {
            (f(args), ...);
        }, values);
    }
};

// Фабрика для создания кортежей
template <typename... Ts>
std::shared_ptr<MonadTuple<Ts...>> make_universal_tuple(Ts... args) {
    return std::make_shared<MonadTuple<Ts...>>(args...);
}