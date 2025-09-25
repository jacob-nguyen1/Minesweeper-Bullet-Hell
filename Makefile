build:
	g++ --std=c++11 *.cpp -o minesweeper -I. -I SFML/include -L SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
run: build
	./minesweeper
