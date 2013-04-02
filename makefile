all: view-distribution encrypt-example


encrypt-example: encrypt-example.cpp rc4.cpp rc4.h
	g++ -g -O3 encrypt-example.cpp rc4.cpp -std=c++0x -o $@

view-distribution: view-distribution.cpp rc4.cpp rc4.h
	g++ -g -O3 view-distribution.cpp rc4.cpp -std=c++0x -o $@ -lpthread
