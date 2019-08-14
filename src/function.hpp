#pragma once
#include <memory>
#include <functional>

// namespace dragon {
//   class Machine;
// }

template <typename I, typename O, typename C> class Function {
public:
  Function() {
    this->f = [](I) { return O(); };
    this->context = std::make_shared<C>(C());
  }

  template <class T> Function(T t, C c) {
    this->f = std::function<O(I)>(t);
    this->context = std::make_shared<C>(c);
  }

  C get_context() { return *this->context; }

  inline O operator()(I i) { return this->f(i); }

  inline bool operator==(Function) { return false; }
  inline bool operator==(Function) const { return false; }

  inline bool operator!=(Function) { return true; }
  inline bool operator!=(Function) const { return true; }

private:
  std::function<O(I)> f;
  std::shared_ptr<C> context;
};