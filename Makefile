.PHONY: build build_release clean print clip run edit dbg valgrind format winmake winbuild winclean winrun winclip

build:
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} ./; make; echo "DEBUG BUILD"

build_release:
	cd build; cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} ./; make; echo "RELEASE BUILD"

clean:
	rm -rf bin/*
	rm -rf build/CMakeFiles
	rm -rf build/cmake_install.cmake
	rm -rf build/CMakeCache.txt
	rm -rf build/Makefile

print:
	cat ./test.txt | grep -vE "(^#.*)"

clip:
	(cat ./test.txt | grep -vE "(^#.*)") | xclip -sel clip

winclip:
	(cat ./test.txt | grep -vE "(^#.*)") | clip.exe

run:
	./bin/main.x86

make:
	cd ./build; make

edit:
	${EDITOR} ./test.txt

dgb:
	(((cat ./test.txt | grep -vE "(^#.*)") && cat) | ./bin/main.x86)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes --suppressions=./valgrind.supp ./bin/main.x86

format:
	clang-format-7 -i src/*

winmake:
	cd .\build\ & cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ./

winbuild:
	cd .\build\ & mingw32-make.exe

winclean:
	if exist .\bin\main.x86.exe del .\bin\main.x86.exe
	if exist .\build\CMakeFiles rd /s /q .\build\CMakeFiles
	del .\build\cmake_install.cmake
	del .\build\CMakeCache.txt
	del .\build\Makefile

winrun:
	.\bin\main.x86.exe
