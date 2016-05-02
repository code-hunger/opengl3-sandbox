CXX = g++

LIBS = -lglfw -lGL -lGLEW -lGLU
ERR_FLAGS = -Werror -Wall -Wconversion -Wextra -Wpedantic -Weffc++ -Wdouble-promotion -Wsign-conversion 

CXXFLAGS = -std=c++14 -g $(ERR_FLAGS) $(LIBS)

BINARY = 'bin'

bin: main.o 
	$(CXX) $(CXXFLAGS) -o $@ main.o 

clean:
	rm *.o *.h.gch $(BINARY)

main.o: main.cpp Shader.h 
	$(CXX) $(CXXFLAGS) -c main.cpp utils.h  Shader.h

# Shader.h.gch: Shader.h
# 	$(CXX) $(CXXFLAGS) -c Shader.h utils.h
