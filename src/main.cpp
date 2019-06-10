#include "machine.hpp"
#include <map>
#include <iostream>
#include <string>

int main()
{
	auto dragon = Machine();
	dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::Map()); }));
	dragon.push(Object::String("dict"));
	dragon.store();

	dragon.push(Object::Fn([](Machine& dragon) { dragon.push(Object::List({})); }));
	dragon.push(Object::String("list"));
	dragon.store();

	dragon.push(Object::Fn([](Machine& dragon) { 		dragon.push(Object::String("dict"));
	dragon.load();
dragon.call();
	dragon.push(Object::String("self"));
	dragon.store();

		dragon.push(Object::String("dict"));
	dragon.load();
dragon.call();
		dragon.push(Object::String("self"));
	dragon.load();
;dragon.push(Object::String("open"));;dragon.index();
	dragon.assign();

	dragon.push(Object::Fn([](Machine& dragon) { 	dragon.push(Object::String("s"));
	dragon.store();

	dragon.push(Object::String("name"));
	dragon.store();

dragon.push(Object::Fn([](Machine& dragon) { 	dragon.push(Object::String("name"));
	dragon.load();
dragon.push(Object::String("Opening file: "));std::cout << (*dragon.pop()).format() << (*dragon.pop()).format()  << std::endl; }));dragon.call(); }));
		dragon.push(Object::String("self"));
	dragon.load();
;dragon.push(Object::String("open"));;dragon.index();;dragon.push(Object::String("fn"));;dragon.index();
	dragon.assign();

	dragon.push(Object::String("self"));
	dragon.load();
 }));
	dragon.push(Object::String("File"));
	dragon.store();

		dragon.push(Object::String("File"));
	dragon.load();
dragon.call();
	dragon.push(Object::String("f"));
	dragon.store();

dragon.push(Object::String("my_file")); 	dragon.push(Object::String("f"));
	dragon.load();
 	dragon.push(Object::String("f"));
	dragon.load();
;dragon.push(Object::String("open"));;dragon.index();;dragon.push(Object::String("fn"));;dragon.index();dragon.call();
    std::cout << dragon.format() << std::endl;
}

