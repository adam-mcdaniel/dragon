#include "machine.hpp"
#include <map>
#include <iostream>
#include <string>

int main()
{
    auto dragon = Machine();
	dragon.push(Object::Fn([](Machine& dragon) { 

    std::cout << "Hello extern!" << std::endl;





    auto param1 = dragon.pop();

    std::cout << "You said: " << param1->format() << std::endl;

 }));
	dragon.push(Object::String("cxx"));
	dragon.store();

dragon.push(Object::String("I passed a value to a cxx function!")); 	dragon.push(Object::String("cxx"));
	dragon.load();
dragon.call();
    std::cout << dragon.format() << std::endl;
}

