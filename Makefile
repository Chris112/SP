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

buffer1: main.o FileIO.o Operations.o Image.o
	echo Buffer overflowing max resize value to allow for massive resizing. Right now fields are 76 bytes apart, check with GDB before submitting assignment
	setarch `arch` -R ./program resizeaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa 6 test2.ppm output.ppm 
clean:
	rm -rf *o program