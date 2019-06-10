from parser import parse
from options import SCRIPT


print(f'''#include "machine.hpp"
#include <map>
#include <iostream>
#include <string>

int main()
{{
    auto dragon = Machine();
{parse(SCRIPT)}
    std::cout << dragon.format() << std::endl;
}}
''')
