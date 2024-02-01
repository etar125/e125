.PHONY: default build
default: build

build: main asm

main: main.cpp main.hpp
	g++ main.cpp -o bin/main

asm: assembler.cpp
	g++ assembler.cpp -o bin/asm