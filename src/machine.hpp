#pragma once
#include "function.hpp"
#include "option.hpp"
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <variant>


namespace dragon {
    class Object;
    class Machine;

    typedef std::variant<bool, double, std::string, Function<Machine &, void>, std::vector<std::shared_ptr<Object>>, std::map<std::string, std::shared_ptr<Object>>> Value;

    class Machine
    {
    public:
        Machine() {}

        void push(Object);
        void push(std::shared_ptr<Object>);
        std::shared_ptr<Object> pop();

        void load();
        void store();

        void assign();
        void index();

        void call();
        void while_loop();

        void add();
        void sub();
        void mul();
        void div();
        void negate();

        std::string format();

    private:
        std::vector<std::shared_ptr<Object>> stack = {};
        std::map<std::string, std::shared_ptr<Object>> registers;
    };

    enum Type
    {
        Bool,
        Double,
        String,
        List,
        Table,
        FunctionType,
        NoneType
    };

    class Object
    {
    public:
        Object();
        explicit Object(bool);
        explicit Object(int);
        explicit Object(double);
        explicit Object(std::string);
        explicit Object(Function<Machine &, void>);
        explicit Object(std::vector<std::shared_ptr<Object>>);
        explicit Object(std::map<std::string, std::shared_ptr<Object>>);

        inline static Object Bool(bool b = false) { return Object(bool(b)); }
        inline static Object Number(double n = 0) { return Object(double(n)); }
        inline static Object String(std::string s = "") { return Object(s); }
        inline static Object Fn(Function<Machine &, void> f) { return Object(f); }
        inline static Object List(std::vector<std::shared_ptr<Object>> list) { return Object(list); }
        inline static Object Map()
        {
            std::map<std::string, std::shared_ptr<Object>> map;
            return Object(map);
        }

        template <typename T>
        Option<T> get();
        std::string format();
        
        Object clone();
        void push(Object);
        void push(std::shared_ptr<Object>);
        std::shared_ptr<Object> pop();

        operator bool();
        operator double();
        operator std::string();

        void operator()(Machine &);
        std::shared_ptr<Object> operator[](Object);
        Object operator+(Object);
        Object operator-(Object);
        Object operator*(Object);
        Object operator/(Object);
        bool operator==(Object);
        bool operator!=(Object);
        bool operator>(Object);
        bool operator<(Object);
        bool operator>=(Object);
        bool operator<=(Object);

        bool is_none() {return this->type == Type::NoneType;}

    private:
        Type type;
        Value value;
    };
}
