#pragma once
#include "function.hpp"
#include "option.hpp"
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <variant>

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
    void call();

    void load();
    void store();

    void assign();
    void index();

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

    static Object Bool(bool b = false) { return Object(bool(b)); }
    static Object Number(double n = 0) { return Object(double(n)); }
    static Object String(std::string s = "") { return Object(s); }
    static Object Fn(Function<Machine &, void> f) { return Object(f); }
    static Object List(std::vector<std::shared_ptr<Object>> list) { return Object(list); }
    static Object Map()
    {
        std::map<std::string, std::shared_ptr<Object>> map;
        return Object(map);
    }

    template <typename T>
    Option<T> get();
    std::string format();

    operator bool();
    operator double();
    operator std::string();

    void operator()(Machine &);
    std::shared_ptr<Object> operator[](Object);
    Object operator+(Object);
    Object operator-(Object);
    Object operator*(Object);
    Object operator/(Object);

private:
    Type type;
    Value value;
};