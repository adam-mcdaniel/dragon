#include "machine.hpp"
#include <map>
#include <iostream>
#include <string>

int main()
{
    auto dragon = Machine();
	dragon.push(Object::Number(4));dragon.negate(); dragon.push(Object::Number(2));dragon.add();
	dragon.push(Object::String("a"));
	dragon.store();

	dragon.push(Object::Number(3)); dragon.push(Object::Number(2));dragon.mul(); dragon.push(Object::Number(4));dragon.add();
	dragon.push(Object::String("b"));
	dragon.store();

	dragon.push(Object::Number(5)); dragon.push(Object::Number(3)); dragon.push(Object::Number(2));dragon.mul();dragon.add();dragon.negate();
	dragon.push(Object::String("c"));
	dragon.store();

    std::cout << dragon.format() << std::endl;
}

