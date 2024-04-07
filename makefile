.PHONY: default build
default: build

build: main_linux

main_linux: main.cpp main.hpp
	clang++ main.cpp -o bin/main

# я пока не протестил на винде, поэтому пусть будет закомментировано
#main_win:
#	i686-w64-mingw32-g++ main.cpp -o bin/main.exe -Igcc_s_dw2-1 -Istdc++-6

asm_linux: assembler.cpp
	clang++ assembler.cpp -o bin/asm