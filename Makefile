CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Wconversion -Wextra -Wpedantic -Werror -Weffc++ -Wdouble-promotion -Wsign-conversion -lglfw -lGL -lGLEW -lGLU 

BINARY = 'bin'

main: main.o
	$(CXX) $(CXXFLAGS) -o $(BINARY) main.o

clean:
	rm *.o $(BINARY)

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp
