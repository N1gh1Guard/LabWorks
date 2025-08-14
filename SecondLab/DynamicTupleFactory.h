#pragma once
#include "MonadTuple.h"
#include <vector>
#include <any>

class DynamicTupleFactory {
public:
    template <typename T>
    static std::shared_ptr<TupleBase> create_tuple(const std::vector<T>& values) {
        return create_tuple_impl(values, std::make_index_sequence<sizeof...(Ts)>{});
    }

private:
    template <typename T, size_t... Is>
    static std::shared_ptr<TupleBase> create_tuple_impl(const std::vector<T>& values, 
                                                        std::index_sequence<Is...>) {
        return make_universal_tuple(values[Is]...);
    }
};