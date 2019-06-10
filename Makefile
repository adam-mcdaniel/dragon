all:
	g++ main.cpp machine.cpp -std=c++17 -o main -O3
	./main


# clang:
# Machine:
# 	[]
# 	{method : <Fn>, test : {a : [], n : 5}}
# g++:
# Machine:
#   []
#   {method : <Fn>, test : {a : [], n : 5}}