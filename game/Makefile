CC = g++

CFLAGS = -c 
OBJ = main.o Maze.o GameObject.o GamePlayer.o Guard.o Key.o MazeDoor.o Trap.o Room.o Position.o


install: $(OBJ)
	$(CC) $(OBJ) -o game `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lpthread

main.o: main.cpp GamePlayer.h Maze.h 
	$(CC) $(CFLAGS) main.cpp 

Maze.o: Maze.cpp Maze.h
	$(CC) $(CFLAGS) Maze.cpp

GameObject.o: GameObject.cpp GameObject.h 
	$(CC) $(CFLAGS) GameObject.cpp

GamePlayer.o: GamePlayer.cpp GamePlayer.h
	$(CC) $(CFLAGS) GamePlayer.cpp
			
Guard.o: Guard.cpp Guard.h
	$(CC) $(CFLAGS) Guard.cpp
		
Key.o: Key.cpp Key.h
	$(CC) $(CFLAGS) Key.cpp

MazeDoor.o: MazeDoor.cpp MazeDoor.h
	$(CC) $(CFLAGS) MazeDoor.cpp
		
Trap.o: Trap.cpp Trap.h
	$(CC) $(CFLAGS) Trap.cpp

Room.o: Room.cpp Room.h
	$(CC) $(CFLAGS) Room.cpp 
	
Position.o: Position.cpp Position.h
	$(CC) $(CFLAGS) Position.cpp

clean : 
			rm -rf *.o install
