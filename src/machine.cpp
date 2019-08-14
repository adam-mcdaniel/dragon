#include "function.hpp"
#include "option.hpp"
#include "machine.hpp"
#include <variant>
#include <sstream>
#include <iostream>

dragon::Object::Object()
{
    this->type = dragon::Type::NoneType;
    this->value = false;
}

dragon::Object::Object(bool b)
{
    this->type = dragon::Type::Bool;
    this->value = b;
}

dragon::Object::Object(int n)
{
    this->type = dragon::Type::Double;
    this->value = double(n);
}

dragon::Object::Object(double n)
{
    this->type = Type::Double;
    this->value = n;
}

dragon::Object::Object(std::string s)
{
    this->type = dragon::Type::String;
    this->value = s;
}

dragon::Object::Object(Function<dragon::Machine &, void, Machine> f)
{
    this->type = Type::FunctionType;
    this->value = f;
}

dragon::Object::Object(std::vector<std::shared_ptr<dragon::Object>> list)
{
    this->type = Type::List;
    this->value = list;
}

dragon::Object::Object(std::map<std::string, std::shared_ptr<Object>> table)
{
    this->type = Type::Table;
    this->value = table;
}

template <typename T>
Option<T> dragon::Object::get()
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

std::string dragon::Object::format()
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
    return "None";
}


dragon::Object dragon::Object::clone()
{
    switch (this->type)
    {
    case Type::List:
        {
            auto result = Object::List({});
            for (auto obj : this->get<std::vector<std::shared_ptr<Object>>>().unwrap()) {
                result.push(*obj);
            }
            return result;
        }
        break;
    case Type::Table:
        {
            auto result = Object::Map();
            for (auto pair : this->get<std::map<std::string, std::shared_ptr<Object>>>().unwrap())
            {
                *result[Object::String(pair.first)] = *std::make_shared<Object>(pair.second->clone());
            }
            return result;
        }
        break;
    default:    
        return Object(*this);
        break;
    }
}


void dragon::Object::push(dragon::Object operand)
{
    this->push(std::make_shared<Object>(operand));
}


void dragon::Object::push(std::shared_ptr<dragon::Object> operand)
{
    switch (this->type)
    {
    case Type::List:
        {
            auto list = this->get<std::vector<std::shared_ptr<Object>>>().unwrap();
            list.push_back(operand);
            this->value = list;
        }
        break;
    default:    
        break;
    }
}


std::shared_ptr<dragon::Object> dragon::Object::pop()
{
    switch (this->type)
    {
    case Type::List:
        {
            auto list = this->get<std::vector<std::shared_ptr<Object>>>().unwrap();
            if (list.size() > 0) {
                auto result = list.back();
                list.pop_back();
                this->value = list;
                return result;
            }
        }
        break;
    default:    
        break;
    }
    return std::make_shared<Object>(Object());
}


dragon::Object::operator bool()
{
    switch (this->type)
    {
    case Type::Bool:
        return this->get<bool>().unwrap();
    case Type::Double:
        return bool(this->get<double>().unwrap());
    default:
        return false;
    }
}

dragon::Object::operator double()
{
    auto result = this->get<double>();
    if (result)
    {
        return result.unwrap();
    }
    return 0.0;
}

dragon::Object::operator std::string()
{
    auto result = this->get<std::string>();
    if (result)
    {
        return result.unwrap();
    }
    return "";
}

void dragon::Object::operator()(Machine &m)
{
    auto result = this->get<Function<Machine &, void, Machine>>();
    if (result)
    {
        Function<Machine &, void, Machine> f = result.unwrap();
        f(m);
    }
}

std::shared_ptr<dragon::Object> dragon::Object::operator[](Object index)
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

dragon::Object dragon::Object::operator+(Object o)
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

dragon::Object dragon::Object::operator*(Object o)
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

dragon::Object dragon::Object::operator-(Object o)
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

dragon::Object dragon::Object::operator/(Object o)
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

bool dragon::Object::operator==(Object o)
{
    if (this->type == o.type) {
        return this->value == o.value;
    }
    return false;
}

bool dragon::Object::operator!=(Object o)
{
    return this->value != o.value;
}

void dragon::Machine::clone()
{
    this->push(this->pop()->clone());
}

void dragon::Machine::push(Object o)
{
    this->stack->push_back(std::make_shared<Object>(o));
}

void dragon::Machine::push(std::shared_ptr<Object> o)
{
    this->stack->push_back(o);
}

std::shared_ptr<dragon::Object> dragon::Machine::pop()
{
    if (this->stack->size() > 0) {
        auto result = this->stack->back();
        this->stack->pop_back();
        return result;
    } else {
        return std::make_shared<Object>(Object());
    }
}

void dragon::Machine::add()
{
    auto a = *this->pop();
    auto b = *this->pop();
    this->push(b + a);
}

void dragon::Machine::sub()
{
    auto a = *this->pop();
    auto b = *this->pop();
    this->push(a - b);
}

void dragon::Machine::mul()
{
    auto a = *this->pop();
    auto b = *this->pop();
    this->push(b * a);
}

void dragon::Machine::div()
{
    auto a = *this->pop();
    auto b = *this->pop();
    this->push(a / b);
}

void dragon::Machine::negate()
{
    auto a = *this->pop();
    this->push(Object::Number(-(a.get<double>().unwrap())));
}


void dragon::Machine::call()
{
    auto f = this->pop();
    Function<Machine &, void, Machine> function = f->get<Function<Machine &, void, Machine>>().unwrap();

    Machine temp_machine = function.get_context();
    temp_machine.stack = this->stack;

    (*f)(temp_machine);
    this->stack = temp_machine.stack;
}


void dragon::Machine::method_call()
{
    // Machine temp_machine = Machine(*this);

    auto index = this->pop();
    auto object = this->pop();
    this->push(object);

    Function<Machine &, void, Machine> function = ((*object)[*index])->get<Function<Machine &, void, Machine>>().unwrap();

    Machine temp_machine = function.get_context();
    temp_machine.stack = this->stack;

    (function)(temp_machine);
    this->stack = temp_machine.stack;
}

void dragon::Machine::while_loop()
{
    auto condition_f = this->pop();
    auto body_f = this->pop();

    (*condition_f)(*this);
    while (*this->pop()) {
        (*body_f)(*this);
        (*condition_f)(*this);
    }
}

void dragon::Machine::assign()
{
    std::shared_ptr<Object> ptr = this->pop();
    std::shared_ptr<Object> value = this->pop();
    *ptr = *value;
}

void dragon::Machine::index()
{
    std::shared_ptr<Object> key = this->pop();
    std::shared_ptr<Object> value = this->pop();
    this->push((*value)[*key]);
}

void dragon::Machine::store()
{
    std::string key = this->pop()->get<std::string>().unwrap();
    std::shared_ptr<Object> value = this->pop();
    (*this->registers)[key] = value;
}

void dragon::Machine::load()
{
    std::string key = this->pop()->get<std::string>().unwrap();

    auto iter = this->registers->find(key);
    if (iter == this->registers->end())
    {
        this->registers->insert(
            std::pair<std::string, std::shared_ptr<Object>>(
                key, std::make_shared<Object>(Object())));
    }

    this->push((*this->registers)[key]);
}

std::string dragon::Machine::format()
{
    // std::vector<std::shared_ptr<Object>> stack = {};
    // std::map<std::string, std::shared_ptr<Object>> registers;
    std::string result = "Machine:\n  [";
    bool pop = false;
    for (auto obj : *this->stack)
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
    for (auto pair : *my_map)
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
