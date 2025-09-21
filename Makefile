all: 
	gcc src/*.c -lSDL2 -lSDL2_ttf

debug: 
	gcc src/*.c -lSDL2 -lSDL2_ttf -Wall -g
