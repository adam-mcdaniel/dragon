# dragon

A statically compiled, dynamically typed programming language.

## Getting Started

Install dependencies.

```bash
# Ubuntu
sudo apt-get install make

# The version on apt is outdated, download from cmake.org
sudo apt-get install cmake

sudo apt-get install python3-dev
sudo apt-get install python3-pip
python3 -m pip install lark-parser
```

Clone the repo.

```bash
git clone "https://github.com/adam-mcdaniel/dragon"

cd dragon
```

Build dragon.

```bash
mkdir build
cd build

cmake ..
```

Compile a script.

```bash
# in build directory
cat ../examples/script.dn | python ../compiler.py > ../src/main.cpp;

make
./dragon
```
