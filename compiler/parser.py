from lark import Lark, Transformer


GRAMMAR = r'''
block: (instruction ";")*
instruction: while_loop | store | print | value

// INSTRUCTIONS
store: name "=" [value] -> store
     | index "=" [value] -> assign

print: "|" (value)*
call: value "(" (value ",")* value? ")"
while_loop: "while" func "do" func

negate.0: "-" value
add.2: value "+" value
sub.2: value "-" value
mul.1: value "*" value
div.1: value "/" value

math: add | sub | mul | div | negate

// VALUES
value: "(" ")" -> dict
     | "[" "]" -> list
     | extern
     | func
     | call
     | math
     | bool
     | string
     | number
     | index
     | "@" -> none
     | name -> load
     | "(" value ")"

index: value "[" (number | string) "]"

func: "{" block "}"
extern: EXTERN
name: NAME
string: STRING
bool: "true" -> true | "false" -> false
number: NUMBER | FLOAT_NUMBER

NUMBER: /0|[1-9]\d*/i
FLOAT_NUMBER: /((\d+\.\d*|\.\d+)(e[-+]?\d+)?|\d+(e[-+]?\d+))/i
STRING : /[ubf]?r?("(?!"").*?(?<!\\)(\\\\)*?"|'(?!'').*?(?<!\\)(\\\\)*?')/i
NAME: /[a-zA-Z_]\w*/
COMMENT: /#[^\n]*/
EXTERN: /extern\s*(?:[^}]*{[^}]*}|;)/
%ignore /[\t \f\n]+/  // WS
%ignore /\\[\t \f\n]*\r?\n/   // LINE_CONT
%ignore COMMENT
'''
MACHINE_NAME = "dragon"


class DragonParser(Transformer):

    def instruction(self, t): return f"{str(t[0])}"

    def extern(self, t):
        block = t[0]
        block = block[block.find("{")+1:block.rfind("}")]
        return self.func([block])


    def while_loop(
        self, t): return f"{' '.join(t[::-1])}{MACHINE_NAME}.while_loop();"


    def call(
        self, t): return f"{' '.join(t[::-1])}{MACHINE_NAME}.call();"

    def negate(
        self, t): return f"{str(t[0])}{MACHINE_NAME}.negate();"

    def add(
        self, t): return f"{' '.join(t[::-1])}{MACHINE_NAME}.add();"

    def sub(
        self, t): return f"{' '.join(t[::-1])}{MACHINE_NAME}.sub();"

    def div(
        self, t): return f"{' '.join(t[::-1])}{MACHINE_NAME}.div();"

    def mul(
        self, t): return f"{' '.join(t[::-1])}{MACHINE_NAME}.mul();"

    def math(
        self, t): return f"{str(t[0])}"

    def string(
        self, t): return f"{MACHINE_NAME}.push(Object::String({str(t[0])}));"

    def true(
        self, t): return f"{MACHINE_NAME}.push(Object::Bool(true));"

    def false(
        self, t): return f"{MACHINE_NAME}.push(Object::Bool(false));"

    def none(
        self, t): return f"{MACHINE_NAME}.push(Object());"

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


def parse(text): return str(DragonParser().transform(
    Lark(
        GRAMMAR,
        start='block',
        parser='lalr',
        lexer='standard'
    ).parse(text)
))
