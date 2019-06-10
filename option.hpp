#pragma once

template<typename T>
class Option {
    T t;
    bool is_some;
    Option(T t) { this->t = t; this->is_some = true; }
    Option() { this->is_some = true; }
public:
    static Option<T> Some(T t) {
        return Option(t);
    }

    static Option<T> None() {
        return Option();
    }

    operator bool() {
        return this->is_some;
    }

    T unwrap() {
        return this->t;
    }
};