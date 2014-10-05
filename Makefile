CCSEPAssignment: main.o FileIO.o Operations.o Image.o
	g++ main.o FileIO.o Operations.o Image.o -o CCSEPAssignment

main.o: main.cpp
	g++ -c -g main.cpp

Image.o: Image.hpp Image.cpp
	g++ -c -g Image.hpp Image.cpp

FileIO.o: FileIO.cpp FileIO.hpp
	g++ -c -g FileIO.cpp FileIO.hpp

Operations.o: Operations.cpp Operations.hpp
	g++ -c -g Operations.cpp Operations.hpp

buffer1: main.o FileIO.o Operations.o Image.o
	setarch `arch` -R ./CCSEPAssignment resizeaaaaaaaaaaa 6 test2.ppm output.ppm 


disabled: main.o FileIO.o Operations.o Image.o
	g++ -fno-stack-protector main.o FileIO.o Operations.o Image.o -o CCSEPAssignment

clean:
	rm -rf *o *.gch CCSEPAssignment

