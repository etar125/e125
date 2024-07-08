.PHONY: default build
default: build

build: main_linux misc_linux

main_linux:
	g++ src/main.cpp src/extension.cpp -o bin/main -Ltinyss -ltinyss

misc_linux:
	cp -r exts bin
	cp -r lang bin

# я пока не протестил на винде, поэтому пусть будет закомментировано
#main_win:
#	i686-w64-mingw32-g++ main.cpp -o bin/main.exe -Igcc_s_dw2-1 -Istdc++-6

#asm_linux:
#	clang++ extra/assembler.cpp -o bin/asm
