SOURCES := $(wildcard src/*.cpp)

build:
	g++ --std=c++11 $(SOURCES) -o minesweeper -Iinclude -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
