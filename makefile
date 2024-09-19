FLAGS=-Os -ffunction-sections -Wl,--gc-sections,--strip-all -fno-asynchronous-unwind-tables -Wall -Wextra
default: $(shell mkdir -p bin) mainl miscl

updtss:
	rm -rf tinyss
	git clone https://github.com/etar125/tinyss.git
	rm -rf tinyss/.git
	rm -f tinyss/.gitignore
	rm -f tinyss/tinyss.md
	rm -f tinyss/README.md

library: tinyss/makefile
	$(MAKE) -C tinyss library FLAGS="$(FLAGS)"

mainl: src/main.cpp src/main.hpp src/extension.hpp src/str.hpp library
	g++ src/main.cpp -o bin/e125 -Ltinyss/bin -Itinyss/bin -ltinyss $(FLAGS)

miscl:
	cp -r exts bin
	cp -r lang bin
	
clean:
	rm -rf $(wildcard bin/*)


#asm_linux:
#	clang++ extra/assembler.cpp -o bin/asm
