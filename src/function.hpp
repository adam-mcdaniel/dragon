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

    inline O operator()(I i)
    {
        return this->f(i);
    }

    inline bool operator==(Function) {return false;}
    inline bool operator==(Function) const {return false;}

    inline bool operator!=(Function) {return true;}
    inline bool operator!=(Function) const {return true;}

private:
    std::function<O(I)> f;
};