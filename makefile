all:
	mkdir -p bin
	gcc -Wall -o bin/mkmatrix mkmatrix.c
	gcc -Wall -o bin/lsmatrix lsmatrix.c
	gcc -Wall -o bin/det det.c
	g++ -Wall -std=c++0x -o bin/gauss gauss.cpp
	g++ -Wall -std=c++0x -o bin/inverse inverse.cpp
	gcc -Wall -o bin/transpose transpose.c
	gcc -Wall -o bin/mult mult.c