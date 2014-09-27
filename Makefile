program: main.o FileIO.o Operations.o Image.o
	g++ main.o FileIO.o Operations.o Image.o -o program

main.o: main.cpp
	g++ -c -g main.cpp

Image.o: Image.hpp Image.cpp
	g++ -c -g Image.hpp Image.cpp

FileIO.o: FileIO.cpp FileIO.hpp
	g++ -c -g FileIO.cpp FileIO.hpp

Operations.o: Operations.cpp Operations.hpp
	g++ -c -g Operations.cpp Operations.hpp

clean:
	rm -rf *o program