output: main.o operations.o
	g++ main.o operations.o -o output

main.o: main.cpp
	g++ -c main.cpp

operations.o: operations.cpp operations.h
	g++ -c operations.cpp

clean:
	rm *.o output