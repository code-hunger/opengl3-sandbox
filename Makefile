CXX = g++

LIBS = -lglfw -lGL -lGLEW -lGLU
ERR_FLAGS = -Werror -Wall -Wconversion -Wextra -Wpedantic -Weffc++ -Wdouble-promotion -Wsign-conversion 

CXXFLAGS = -std=c++14 -g -O0 $(ERR_FLAGS) $(LIBS)

BINARY = 'bin'

bin: main.o 
	$(CXX) $(CXXFLAGS) -o $@ main.o

clean:
	rm *.o *.h.gch $(BINARY)

main.o: main.cpp Shader.h utils.h
	$(CXX) $(CXXFLAGS) -c main.cpp
