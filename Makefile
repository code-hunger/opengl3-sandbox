CXX = g++

DEBUG_FLAGS = -g -O0
LIBS = -lGL -lGLU -lglfw -lGLEW 

ERR_FLAGS = -Wall -Wextra -Wpedantic -Wdouble-promotion \
			-Wconversion -Wsign-conversion -Wlogical-op \
			-Wabi  -Wctor-dtor-privacy \
			-Wnon-virtual-dtor  -Wreorder \
			-Weffc++  -Wstrict-null-sentinel \
			-Wno-non-template-friend  -Wold-style-cast \
			-Woverloaded-virtual  -Wno-pmf-conversions \
			-Wsign-promo -Wno-error=unused-variable

CXXFLAGS = -std=c++14 -Werror $(DEBUG_FLAGS) $(ERR_FLAGS) $(LIBS) 

BINARY = 'bin'

bin: VertexArray.o App.o Window.o main.cpp Shader.o ShaderProgram.o utils.h
	$(CXX) $(CXXFLAGS) -o $@ main.cpp VertexArray.o App.o Window.o ShaderProgram.o Shader.o # @TODO probably move Shader.o before ShaderProgram.o? 

VertexArray.o: VertexArray.h

App.o: App.h utils.h Shader.h ShaderProgram.h

Window.o: Window.h

ShaderProgram.o: ShaderProgram.h 

Shader.o: Shader.h

clean:
	rm $(BINARY) *.o

printflags:
	print $(CXXFLAGS)
