CXX = g++

LIBS = -lglfw -lGL -lGLEW -lGLU
ERR_FLAGS = -Wall -Wconversion -Wextra -Wpedantic -Weffc++ -Wdouble-promotion -Wsign-conversion 

CXXFLAGS = -std=c++14 -g -O0 $(ERR_FLAGS) $(LIBS) -Werror 

BINARY = 'bin'

bin: main.cpp Shader.h ShaderProgram.h utils.h
	$(CXX) $(CXXFLAGS) -o $@ main.cpp

clean:
	rm *.o $(BINARY)
