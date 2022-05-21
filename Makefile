CXX=clang++
CXXFLAGS=-Wall -std=c++11 -g
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
SFML_BASE := $(shell brew info sfml | grep Cellar | cut -f1 -d ' ' )
INCLUDES := $(SFML_BASE)/include
LIBS := $(SFML_BASE)/lib

main: main.cpp Grid.cpp Cell.cpp Global.hpp Grid.hpp Cell.hpp
	$(info $(SFML_BASE))
	$(CXX) *.cpp $(CXXFLAGS) -I$(INCLUDES) $(LDFLAGS) -L$(LIBS) -o main

clean:
	$(RM) main *.o

run: main
	./main
