#pragma once
#include <functional>

template <typename I, typename O>
class Function
{
public:
    Function() {}

    template <class T>
    Function(T t)
    {
        this->f = std::function<O(I)>(t);
    }

    O operator()(I i)
    {
        return this->f(i);
    }

    bool operator==(Function) {return false;}
    bool operator==(Function) const {return false;}

private:
    std::function<O(I)> f;
};