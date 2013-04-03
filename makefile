all: view-distribution encrypt-example decryptor bigrams-decryptor


encrypt-example: encrypt-example.cpp rc4.cpp rc4.h stattables.h
	g++ -g -O3 encrypt-example.cpp rc4.cpp -std=c++0x -o $@

bigrams-decryptor: bigrams-decryptor.cpp stattables.h
	g++ -g -O3 bigrams-decryptor.cpp -std=c++0x -Ifxt/src -o $@

decryptor: decryptor.cpp stattables.h
	g++ -g -O3 decryptor.cpp -std=c++0x -Ifxt/src -o $@

view-distribution: view-distribution.cpp rc4.cpp rc4.h
	g++ -g -O3 view-distribution.cpp rc4.cpp -std=c++0x -o $@ -lpthread
