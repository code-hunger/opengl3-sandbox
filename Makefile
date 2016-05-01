CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Wconversion -Wextra -Wpedantic -Werror -Weffc++ -Wdouble-promotion -Wsign-conversion -lglfw -lGL -lGLEW -lGLU 

BINARY = 'bin'

bin: main.o
	$(CXX) $(CXXFLAGS) -o $@ main.o

clean:
	rm *.o $(BINARY)

main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp
