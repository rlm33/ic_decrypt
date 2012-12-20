main: Decrypter.o
	mpicxx -w main.cpp Decrypter.o -fpermissive -fopenmp -o decrypter

Decrypter.o: Decrypter.h Decrypter.cpp
	mpicxx -w -c Decrypter.cpp -fpermissive -fopenmp

