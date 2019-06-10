#pragma once
#include <functional>

template<typename I, typename O>
class Function {
public:
    Function() {}

    template<class T>
    Function(T t) {
        this->f = std::function(t);
    }

    O operator ()(I i) {
        return this->f(i);
    }
private:
    std::function<O(I)> f;
};