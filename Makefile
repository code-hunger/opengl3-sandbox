CXX = g++

LIBS = -lglfw -lGL -lGLEW -lGLU
ERR_FLAGS = -Wall -Wextra -Wpedantic -Weffc++ -Wdouble-promotion -Wconversion -Wsign-conversion -Wlogical-op
DEBUG_FLAGS = -g -O0
CXXFLAGS = -std=c++14 $(DEBUG_FLAGS) $(ERR_FLAGS) $(LIBS) -Werror 
CXXFLAGS += \
	-Wabi  -Wctor-dtor-privacy  \
	-Wnon-virtual-dtor  -Wreorder  \
	-Weffc++  -Wstrict-null-sentinel  \
	-Wno-non-template-friend  -Wold-style-cast  \
	-Woverloaded-virtual  -Wno-pmf-conversions  \
	-Wsign-promo

BINARY = 'bin'

bin: VertexArray.o App.o Window.o main.cpp Shader.h ShaderProgram.h utils.h
	$(CXX) $(CXXFLAGS) -o $@ main.cpp VertexArray.o App.o Window.o

VertexArray.o: VertexArray.h

App.o: App.h utils.h Shader.h ShaderProgram.h

Window.o: Window.h

clean:
	rm $(BINARY) *.o

printflags:
	print $(CXXFLAGS)
