FLAGS=-Os -ffunction-sections -Wl,--gc-sections,--strip-all -fno-asynchronous-unwind-tables

default: $(shell mkdir -p bin) mainl miscl

mainl: src/main.cpp src/main.hpp src/extension.hpp src/str.hpp tinyss/libtinyss.a tinyss/token.hpp tinyss/tinyss.hpp
	g++ src/main.cpp -o bin/e125 -Ltinyss -ltinyss $(FLAGS)

miscl:
	cp -r exts bin
	cp -r lang bin
	
clean:
	rm -rf $(wildcard bin/*)


#asm_linux:
#	clang++ extra/assembler.cpp -o bin/asm
