#include "function.hpp"
#include "option.hpp"
#include "machine.hpp"
#include <variant>
#include <sstream>
#include <iostream>

Object::Object()
{
    this->type = Type::NoneType;
    this->value = false;
}

Object::Object(bool b)
{
    this->type = Type::Bool;
    this->value = b;
}

Object::Object(int n)
{
    this->type = Type::Double;
    this->value = double(n);
}

Object::Object(double n)
{
    this->type = Type::Double;
    this->value = n;
}

Object::Object(std::string s)
{
    this->type = Type::String;
    this->value = s;
}

Object::Object(Function<Machine &, void> f)
{
    this->type = Type::FunctionType;
    this->value = f;
}

Object::Object(std::vector<std::shared_ptr<Object>> list)
{
    this->type = Type::List;
    this->value = list;
}

Object::Object(std::map<std::string, std::shared_ptr<Object>> table)
{
    this->type = Type::Table;
    this->value = table;
}

template <typename T>
Option<T> Object::get()
{
    try
    {
        return Option<T>::Some(std::get<T>(this->value));
    }
    catch (std::bad_variant_access)
    {
        return Option<T>::None();
    }
}

std::string Object::format()
{
    switch (this->type)
    {
    case Type::Bool:
    {
        if (this->get<bool>().unwrap())
        {
            return "true";
        }
        return "false";
    }
    break;
    case Type::Double:
    {
        double n = this->get<double>().unwrap();
        std::ostringstream ss;
        ss << n;
        return ss.str();
    }
    break;
    case Type::String:
    {
        return this->get<std::string>().unwrap();
    }
    break;
    case Type::List:
    {
        std::string result = "[";
        bool pop = false;
        for (auto obj : this->get<std::vector<std::shared_ptr<Object>>>().unwrap())
        {
            pop = true;
            result += (*obj).format();
            result += ", ";
        }
        if (pop)
        {
            result.pop_back();
            result.pop_back();
        }
        result += "]";
        return result;
    }
    break;
    case Type::Table:
    {
        std::string result = "{";
        auto my_map = this->get<std::map<std::string, std::shared_ptr<Object>>>().unwrap();
        bool pop = false;
        for (auto pair : my_map)
        {
            pop = true;
            result += pair.first;
            result += " : ";
            result += (*pair.second).format();
            result += ", ";
        }
        if (pop)
        {
            result.pop_back();
            result.pop_back();
        }
        result += "}";
        return result;
    }
    break;
    case Type::FunctionType:
    {
        return "<Fn>";
    }
    break;
    case Type::NoneType:
    {
        return "None";
    }
    break;
    }
}

Object::operator bool()
{
    auto result = this->get<bool>();
    if (result)
    {
        return result.unwrap();
    }
    return false;
}

Object::operator double()
{
    auto result = this->get<double>();
    if (result)
    {
        return result.unwrap();
    }
    return 0.0;
}

Object::operator std::string()
{
    auto result = this->get<std::string>();
    if (result)
    {
        return result.unwrap();
    }
    return "";
}

void Object::operator()(Machine &m)
{
    auto result = this->get<Function<Machine &, void>>();
    if (result)
    {
        Function<Machine &, void> f = result.unwrap();
        f(m);
    }
}

std::shared_ptr<Object> Object::operator[](Object index)
{
    double num_index = index.get<double>().unwrap();
    std::string str_index = index.get<std::string>().unwrap();

    if (auto pval = std::get_if<std::vector<std::shared_ptr<Object>>>(&this->value))
    {
        if (num_index >= pval->size())
        {
            pval->push_back(std::make_shared<Object>(Object()));
        }
        return (*pval)[num_index];
    }

    if (auto pval = std::get_if<std::map<std::string, std::shared_ptr<Object>>>(&this->value))
    {
        // if (opt_str_index >= pval->size()) {
        //     pval->push_back(std::make_shared<Object>(Object()));
        // }
        auto iter = pval->find(str_index);
        if (iter == pval->end())
        {
            pval->insert(
                std::pair<std::string, std::shared_ptr<Object>>(
                    str_index,
                    std::make_shared<Object>(Object())));
        }
        return (*pval)[str_index];
    }

    return std::make_shared<Object>(Object());
}

Object Object::operator+(Object o)
{
    switch (this->type)
    {
    case Type::Bool:
    {
        auto their_bool = o.get<bool>().unwrap();
        auto my_bool = this->get<bool>().unwrap();
        return Object(their_bool || my_bool);
    }
    break;
    case Type::Double:
    {
        auto their_num = o.get<double>().unwrap();
        auto my_num = this->get<double>().unwrap();
        return Object(their_num + my_num);
    }
    break;
    case Type::String:
    {
        auto their_str = o.get<std::string>().unwrap();
        auto my_str = this->get<std::string>().unwrap();
        return Object(my_str + their_str);
    }
    break;
    case Type::List:
    {
        auto their_vec = o.get<std::vector<std::shared_ptr<Object>>>().unwrap();
        auto my_vec = this->get<std::vector<std::shared_ptr<Object>>>().unwrap();
        for (auto item : their_vec)
        {
            my_vec.push_back(item);
        }
        return Object(my_vec);
    }
    break;
    case Type::Table:
        return Object();
        break;
    case Type::FunctionType:
        return Object();
        break;
    case Type::NoneType:
        return Object();
        break;
    }
    return Object();
}

Object Object::operator*(Object o)
{
    switch (this->type)
    {
    case Type::Bool:
    {
        auto their_bool = o.get<bool>().unwrap();
        auto my_bool = this->get<bool>().unwrap();
        return Object(bool(their_bool && my_bool));
    }
    break;
    case Type::Double:
    {
        auto their_num = o.get<double>().unwrap();
        auto my_num = this->get<double>().unwrap();
        return Object(double(their_num * my_num));
    }
    break;
    case Type::String:
        return Object();
        break;
    case Type::List:
        return Object();
        break;
    case Type::Table:
        return Object();
        break;
    case Type::FunctionType:
        return Object();
        break;
    case Type::NoneType:
        return Object();
        break;
    }
    return Object();
}

Object Object::operator-(Object o)
{
    switch (this->type)
    {
    case Type::Bool:
        return Object();
        break;
    case Type::Double:
    {
        auto their_num = o.get<double>().unwrap();
        auto my_num = this->get<double>().unwrap();
        return Object(my_num - their_num);
    }
    break;
    case Type::String:
        return Object();
        break;
    case Type::List:
        return Object();
        break;
    case Type::Table:
        return Object();
        break;
    case Type::FunctionType:
        return Object();
        break;
    case Type::NoneType:
        return Object();
        break;
    }
    return Object();
}

Object Object::operator/(Object o)
{
    switch (this->type)
    {
    case Type::Bool:
        return Object();
        break;
    case Type::Double:
    {
        auto their_num = o.get<double>().unwrap();
        auto my_num = this->get<double>().unwrap();
        return Object(my_num / their_num);
    }
    break;
    case Type::String:
        return Object();
        break;
    case Type::List:
        return Object();
        break;
    case Type::Table:
        return Object();
        break;
    case Type::FunctionType:
        return Object();
        break;
    case Type::NoneType:
        return Object();
        break;
    }
    return Object();
}

void Machine::push(Object o)
{
    this->stack.push_back(std::make_shared<Object>(o));
}

void Machine::push(std::shared_ptr<Object> o)
{
    this->stack.push_back(o);
}

std::shared_ptr<Object> Machine::pop()
{
    this->num_pops += 1;
    auto result = this->stack.back();
    this->stack.pop_back();
    return result;
}

void Machine::call()
{
    Machine temp_machine = Machine(*this);

    auto f = temp_machine.pop();
    (*f)(temp_machine);
    // std::cout << "POPS " << temp_machine.num_pops << std::endl;
    // for (int i = 0; i < temp_machine.num_pops; i++)
    // {
    //     this->pop();
    // }
    this->stack = temp_machine.stack;
    // for (int i = 0; i < temp_machine.num_pushes; i++)
    // {
    //     this->push(temp_machine.pop());
    // }
}

void Machine::assign()
{
    std::shared_ptr<Object> ptr = this->pop();
    std::shared_ptr<Object> value = this->pop();
    *ptr = *value;
}

void Machine::index()
{
    std::shared_ptr<Object> key = this->pop();
    std::shared_ptr<Object> value = this->pop();
    this->push((*value)[*key]);
}

void Machine::store()
{
    std::string key = this->pop()->get<std::string>().unwrap();
    std::shared_ptr<Object> value = this->pop();
    this->registers[key] = value;
}

void Machine::load()
{
    std::string key = this->pop()->get<std::string>().unwrap();

    auto iter = this->registers.find(key);
    if (iter == this->registers.end())
    {
        this->registers.insert(
            std::pair<std::string, std::shared_ptr<Object>>(
                key, std::make_shared<Object>(Object())));
    }

    this->push(this->registers[key]);
}

std::string Machine::format()
{
    // std::vector<std::shared_ptr<Object>> stack = {};
    // std::map<std::string, std::shared_ptr<Object>> registers;
    std::string result = "Machine:\n  [";
    bool pop = false;
    for (auto obj : this->stack)
    {
        pop = true;
        result += (*obj).format();
        result += ", ";
    }

    if (pop)
    {
        result.pop_back();
        result.pop_back();
    }
    result += "]\n  {";

    auto my_map = this->registers;
    pop = false;
    for (auto pair : my_map)
    {
        pop = true;
        result += pair.first;
        result += " : ";
        result += (*pair.second).format();
        result += ", ";
    }
    if (pop)
    {
        result.pop_back();
        result.pop_back();
    }
    result += "}";

    return result;
}