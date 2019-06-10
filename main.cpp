#include "machine.hpp"
#include <map>
#include <iostream>
#include <string>

// using std::string;
// using Object;
// using Machine;

int main()
{
	//     std::cout
	//         << (Object(20) - Object(10)).get<double>().unwrap()
	//         << std::endl;
	// std::map<string, std::shared_ptr<Object>> map;
	// (*map["test"]) = std::make_shared<Object>(Object(string("ass")));
	// auto dict = Object::Map();
	// *dict[Object(std::string("test"))] = Object(std::string("ass bitch"));
	// // auto function = Object
	// // (*dict[string("test")].unwrap()) = Object(string("ass"));

	// auto dragon = Machine();
	// dragon.push(Object(std::string("test")));
	// dragon.push(Object(double(4.57)));
	// dragon.push(Object(bool(false)));

	// dragon.push(Object::Fn([](Machine& m) {m.push(Object(std::string("whoa")));}));
	// dragon.call();

	// dragon.push(dict);
	// dragon.push(Object());
	// std::cout << dragon.format() << std::endl;
	// dragon.push(Object(Function<Machine&, void>([](Machine& m) {
	// 	m.push(Object("test"));
	// })));
	// dragon.call();
	// std::cout << (*dragon.pop()).get<std::string>().unwrap() << std::endl;

	auto dragon = Machine();
	// mach.push(Object(double(5)));
	// mach.push(Object(std::string("eax")));
	// mach.store();
	// mach.push(Object(std::string("eax")));
	// mach.load();

	// mach.push(Object::Map());
	// mach.push(Object(std::string("eax")));
	// mach.store();
	// mach.push(Object(std::string("eax")));
	// mach.load();
	// mach.push(Object(std::string("test")));
	// mach.index();
	// mach.push(Object(std::string("whoa")));
	// mach.store();
	// mach.push(Object(std::string("whoa")));
	// mach.load();
	// mach.push(Object(double(2)));
	// mach.assign();
	// mach.push(Object(std::string("whoa")));
	// mach.load();
	// mach.push(Object(double(3)));
	// mach.assign();
	// mach.push(Object(double(5)));
	// mach.push(Object(std::string("whoa")));
	// mach.store();
	// dragon.push(Object::Fn([](Machine &dragon) { dragon.push(Object::String("Hello, world!"));std::cout << (*dragon.pop()).format()  << std::endl; }));
	// dragon.call();       dragon.push(Object::Map());
	dragon.push(Object::Map());
	dragon.push(Object::String("test"));
	dragon.store();

	dragon.push(Object::Fn([](Machine &dragon) {
		dragon.push(Object::String("self"));
		dragon.store();

		dragon.push(Object::Number(5));
		dragon.push(Object::String("self"));
		dragon.load();
		;
		dragon.push(Object::String("n"));
		;
		dragon.index();
		dragon.assign();

		dragon.push(Object::List({}));
		dragon.push(Object::String("self"));
		dragon.load();
		;
		dragon.push(Object::String("a"));
		;
		dragon.index();
		dragon.assign();

		dragon.push(Object::Number(5));
		dragon.push(Object::String("self"));
		dragon.store();
	}));
	dragon.push(Object::String("method"));
	dragon.store();

	dragon.push(Object::String("test"));
	dragon.load();
	dragon.push(Object::String("method"));
	dragon.load();
	dragon.call();

	std::cout << dragon.format() << std::endl;
}