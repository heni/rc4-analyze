all: view-distribution encrypt-example maxpos-decryptor unigrams-decryptor bigrams-decryptor

encrypt-example: encrypt-example.cpp rc4.cpp rc4.h stattables.h
	g++ -g -O3 encrypt-example.cpp rc4.cpp -std=c++0x -o $@

bigrams-decryptor: bigrams-decryptor.cpp stattables.h
	g++ -g -O3 $< -std=c++0x -Ifxt/src -o $@

unigrams-decryptor: unigrams-decryptor.cpp stattables.h
	g++ -g -O3 $< -std=c++0x -Ifxt/src -o $@

maxpos-decryptor: maxpos-decryptor.cpp stattables.h
	g++ -g -O3 $< -std=c++0x -Ifxt/src -o $@

view-distribution: view-distribution.cpp rc4.cpp rc4.h
	g++ -g -O3 view-distribution.cpp rc4.cpp -std=c++0x -o $@ -lpthread
