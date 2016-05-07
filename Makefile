CXX = g++

LIBS = -lglfw -lGL -lGLEW -lGLU
ERR_FLAGS = -Wall -Wconversion -Wextra -Wpedantic -Weffc++ -Wdouble-promotion -Wsign-conversion 

CXXFLAGS = -std=c++14 -g -O0 $(ERR_FLAGS) $(LIBS) -Werror 

BINARY = 'bin'

bin: VertexArray.o App.o Window.o main.cpp Shader.h ShaderProgram.h utils.h CrashHandler.h
	$(CXX) $(CXXFLAGS) -o $@ main.cpp VertexArray.o App.o Window.o

VertexArray.o: VertexArray.h

App.o: App.h

Window.o: Window.h

clean:
	rm $(BINARY) *.o
