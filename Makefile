CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Wconversion -Wextra -Wpedantic -Werror -Weffc++ -Wdouble-promotion -Wsign-conversion -lglfw -lGL -lGLEW -lGLU 

BINARY = 'bin'

bin: main.o #main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.o
	# g++ main.cpp -o bin -lglfw -lGL -lGLEW

clean:
	rm *.o $(BINARY)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp
