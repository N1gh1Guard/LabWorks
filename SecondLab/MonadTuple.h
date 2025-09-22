#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include <string>
#include <sstream>
#include <utility>
#include <cassert>

// Базовый класс для полиморфизма
class TupleBase {
public:
    virtual size_t size() const = 0;
    virtual std::string toString() const = 0;
    virtual std::unique_ptr<TupleBase> clone() const = 0;
    virtual ~TupleBase() = default;
};

// Вспомогательный класс для хранения элементов с type erasure
class TupleElement {
public:
    template <typename T>
    TupleElement(T value) : 
        data_(new T(std::move(value))),
        deleter_([](void* ptr) { delete static_cast<T*>(ptr); }),
        copier_([](void* ptr) -> void* { return new T(*static_cast<T*>(ptr)); }),
        toString_([](void* ptr) -> std::string { 
            std::stringstream ss;
            ss << *static_cast<T*>(ptr);
            return ss.str();
        })
    {}
    
    TupleElement(const TupleElement& other) :
        data_(other.copier_(other.data_)),
        deleter_(other.deleter_),
        copier_(other.copier_),
        toString_(other.toString_)
    {}
    
    TupleElement(TupleElement&& other) noexcept :
        data_(other.data_),
        deleter_(std::move(other.deleter_)),
        copier_(std::move(other.copier_)),
        toString_(std::move(other.toString_))
    {
        other.data_ = nullptr;
    }
    
    TupleElement& operator=(const TupleElement& other) {
        if (this != &other) {
            if (deleter_) deleter_(data_);
            data_ = other.copier_(other.data_);
            deleter_ = other.deleter_;
            copier_ = other.copier_;
            toString_ = other.toString_;
        }
        return *this;
    }
    
    TupleElement& operator=(TupleElement&& other) noexcept {
        if (this != &other) {
            if (deleter_) deleter_(data_);
            data_ = other.data_;
            deleter_ = std::move(other.deleter_);
            copier_ = std::move(other.copier_);
            toString_ = std::move(other.toString_);
            other.data_ = nullptr;
        }
        return *this;
    }
    
    ~TupleElement() {
        if (deleter_) deleter_(data_);
    }
    
    template <typename T>
    T get() const {
        if (!data_) {
            throw std::runtime_error("Accessing moved-from TupleElement");
        }
        return *static_cast<T*>(data_);
    }
    
    std::string toString() const {
        return toString_(data_);
    }
    
    std::unique_ptr<TupleElement> clone() const {
        return std::unique_ptr<TupleElement>(new TupleElement(*this));
    }

private:
    void* data_ = nullptr;
    std::function<void(void*)> deleter_;
    std::function<void*(void*)> copier_;
    std::function<std::string(void*)> toString_;
};

// Реализация кортежа
template <typename... Ts>
class MonadTuple : public TupleBase {
private:
    std::vector<TupleElement> elements;
    
    // Вспомогательная функция для проверки индекса
    void check_index(size_t index) const {
        if (index >= elements.size()) {
            throw std::out_of_range("Tuple index out of range");
        }
    }

public:
    MonadTuple(Ts... values) {
        elements.reserve(sizeof...(Ts));
        // Используем раскрытие пакета параметров
        int dummy[] = { (elements.push_back(TupleElement(values)), 0)... };
        (void)dummy; // подавляем предупреждение о неиспользуемой переменной
    }
    
    MonadTuple(const MonadTuple& other) {
        elements.reserve(other.elements.size());
        for (const auto& element : other.elements) {
            elements.push_back(*element.clone());
        }
    }
    
    MonadTuple(MonadTuple&& other) noexcept = default;
    
    MonadTuple& operator=(const MonadTuple& other) {
        if (this != &other) {
            elements.clear();
            elements.reserve(other.elements.size());
            for (const auto& element : other.elements) {
                elements.push_back(*element.clone());
            }
        }
        return *this;
    }
    
    MonadTuple& operator=(MonadTuple&& other) noexcept = default;
    
    size_t size() const override {
        return elements.size();
    }
    
    // Получение значения по индексу с правильным типом
    template <size_t I>
    auto get() const {
        check_index(I);
        using T = std::tuple_element_t<I, std::tuple<Ts...>>;
        return elements[I].get<T>();
    }
    
    std::string toString() const override {
        std::string result = "(";
        for (size_t i = 0; i < elements.size(); i++) {
            if (i > 0) result += ", ";
            result += elements[i].toString();
        }
        result += ")";
        return result;
    }
    
    std::unique_ptr<TupleBase> clone() const override {
        return std::unique_ptr<MonadTuple<Ts...>>(new MonadTuple<Ts...>(*this));
    }
};

// Фабрика для создания кортежей
template <typename... Ts>
std::shared_ptr<MonadTuple<Ts...>> make_universal_tuple(Ts... args) {
    return std::shared_ptr<MonadTuple<Ts...>>(new MonadTuple<Ts...>(args...));
}