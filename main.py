from sys import argv
from lark import Lark, Transformer

GRAMMAR = r'''

block: (instruction ";")*
instruction: store | print | value

// INSTRUCTIONS
store: name "=" [value] -> store
     | index "=" [value] -> assign

print: "|>" (value)*
call: value "(" (value ",")* value? ")"

// VALUES
value: "(" ")" -> dict
     | "[" "]" -> list
     | "{" block "}" -> func
     | call
     | string
     | number
     | index
     | name -> load

index: value "[" (number | string) "]"

name: NAME
string: STRING
number: NUMBER | FLOAT_NUMBER

NUMBER: /0|[1-9]\d*/i
FLOAT_NUMBER: /((\d+\.\d*|\.\d+)(e[-+]?\d+)?|\d+(e[-+]?\d+))/i
STRING : /[ubf]?r?("(?!"").*?(?<!\\)(\\\\)*?"|'(?!'').*?(?<!\\)(\\\\)*?')/i
NAME: /[a-zA-Z_]\w*/
COMMENT: /#[^\n]*/
%ignore /[\t \f\n]+/  // WS
%ignore /\\[\t \f\n]*\r?\n/   // LINE_CONT
%ignore COMMENT
'''
MACHINE_NAME = "dragon"


class DragonParser(Transformer):

    def instruction(self, t): return f"{str(t[0])}"

    def call(
        self, t): return f"{' '.join(t[::-1])}{MACHINE_NAME}.call();"

    def string(
        self, t): return f"{MACHINE_NAME}.push(Object::String({str(t[0])}));"

    def name(
        self, t): return f"{MACHINE_NAME}.push(Object::String(\"{str(t[0])}\"));"

    def number(
        self, t): return f"{MACHINE_NAME}.push(Object::Number({str(t[0])}));"

    def index(
        self, t): return f"{str(t[0])};{str(t[1])};{MACHINE_NAME}.index();"

    def print(self, tokens):
        return self.func([f"{''.join(tokens)}std::cout {'<< (*dragon.pop()).format() '*len(tokens)} << std::endl;"]) + f"{MACHINE_NAME}.call();"

    def value(self, tokens):
        return f"{str(tokens[0])}"

    def list(self, tokens):
        return f"{MACHINE_NAME}.push(Object::List({{}}));"

    def dict(self, tokens):
        return f"{MACHINE_NAME}.push(Object::Map());"

    def func(self, tokens):
        return f"{MACHINE_NAME}.push(Object::Fn([](Machine& {MACHINE_NAME}) {{ {str(tokens[0])} }}));"

    def load(self, tokens):
        return f"\t{str(tokens[0])}\n\t{MACHINE_NAME}.load();\n"

    def assign(self, tokens):
        return f"\t{str(tokens[1])}\n\t{str(tokens[0])}\n\t{MACHINE_NAME}.assign();\n"

    def store(self, tokens):
        if len(tokens) > 1:
            return f"\t{str(tokens[1])}\n\t{str(tokens[0])}\n\t{MACHINE_NAME}.store();\n"
        else:
            return f"\t{str(tokens[0])}\n\t{MACHINE_NAME}.store();\n"

    def block(self, tokens):
        return '\n'.join(map(str, tokens))


def parse(text): return DragonParser().transform(
    Lark(
        GRAMMAR,
        start='block',
        parser='lalr',
        lexer='standard'
    ).parse(text)
)


if len(argv) > 1:
    print(f'''#include "machine.hpp"
#include <map>
#include <iostream>
#include <string>

int main()
{{
    {str(parse(sys.argv[1]))}
}}
''')
else:
    print("No input script")
