#include "machine.hpp"
#include <map>
#include <iostream>
#include <string>

int main()
{
    auto dragon = Machine();
	dragon.push(Object::Number(0));
	dragon.push(Object::String("a"));
	dragon.store();

dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::String("testing"));std::cout << (*dragon.pop()).format()  << std::endl; }));dragon.call(); })); dragon.push(Object::Fn([](Machine& dragon) { 	dragon.push(Object::String("a"));
	dragon.load();
 }));dragon.while_loop();
	dragon.push(Object::Bool(false));
	dragon.push(Object::String("a"));
	dragon.store();

dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::String("testing"));std::cout << (*dragon.pop()).format()  << std::endl; }));dragon.call(); })); dragon.push(Object::Fn([](Machine& dragon) { 	dragon.push(Object::String("a"));
	dragon.load();
 }));dragon.while_loop();
	dragon.push(Object::Number(0.5));
	dragon.push(Object::String("a"));
	dragon.store();

dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::String("testing"));std::cout << (*dragon.pop()).format()  << std::endl; }));dragon.call(); })); dragon.push(Object::Fn([](Machine& dragon) { 	dragon.push(Object::String("a"));
	dragon.load();
 }));dragon.while_loop();
	dragon.push(Object::Number(4));
	dragon.push(Object::String("a"));
	dragon.store();

dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::String("testing"));std::cout << (*dragon.pop()).format()  << std::endl; }));dragon.call(); })); dragon.push(Object::Fn([](Machine& dragon) { 	dragon.push(Object::String("a"));
	dragon.load();
 }));dragon.while_loop();
    std::cout << dragon.format() << std::endl;
}

