# dragon

A statically compiled, dynamically typed programming language.

## Getting Started

### Install with rapture

Install rapture

```bash
# Install with cargo to build
cargo install rapture

# Bootstrap rapture
rapt install https://raw.githubusercontent.com/adam-mcdaniel/rapture/master/scripts/rapture.rapt

# Uninstall old cargo
cargo uninstall rapture

# Source your modified path
. ~/.bashrc
source ~/.bashrc
```

Install dragon with rapture
```bash
rapt install https://raw.githubusercontent.com/adam-mcdaniel/dragon/master/scripts/dragon.rapt
```


### Build from source


Install dependencies.

```bash
# Ubuntu
sudo apt-get install make
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
cat ../examples/script.dn | python3 ../compiler/main.py > ../src/main.cpp;

make
./dragon
```
