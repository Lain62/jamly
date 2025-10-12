CXX_FLAGS=-std=c++20 -Wall -Wextra -I./vendor/raylib-5.5_linux_amd64/include -I./vendor/flag -O3
LD_FLAGS=-L./vendor/raylib-5.5_linux_amd64/lib -l:libraylib.a -lm

jamly: jamly.cpp
	clang++ $(CXX_FLAGS) -o jamly jamly.cpp $(LD_FLAGS)
