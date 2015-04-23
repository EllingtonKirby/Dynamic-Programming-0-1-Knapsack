comp = g++
flags = -g -std=c++0x
proj = program3

all: Driver.o Cards.o
	$(comp) $(flags) -o $(proj) Driver.o Cards.o

Driver.o: Driver.cpp 
	$(comp) $(flags) -c Driver.cpp

Cards.o: Cards.cpp
	$(comp) $(flags) -c Cards.cpp

clean: 
	rm *.o $(proj)
