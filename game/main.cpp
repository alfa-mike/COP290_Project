#include <iostream> 
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stack>

#include "SDL2/SDL.h"
#include "Room.h"

#define WIDTH      600
#define HEIGHT     600
#define ROWS       100
#define COLUMNS    100
#define ROOM_WIDTH (WIDTH / ROWS)
#define FPS        512

using namespace std;

bool unvisitedCells(vector <Room> mazeGrid) {
  for (Uint32 i = 0; i < mazeGrid.size(); i++) {
    if (!mazeGrid[i].isVisited()) {
      return true;
    }
  }
  return false;
}

int checkNeighbours(vector <Room> maze, Room r) {
  int x = r.getX();
  int y = r.getY();
  vector < Room > neighbours;
  if(!maze[(x - 1) * ROWS + y].isVisited() && x > 0) {
    neighbours.push_back(maze[(x - 1) * ROWS + y]);
  }
  if(!maze[(x + 1) * ROWS + y].isVisited() && x < ROWS - 1) {
    neighbours.push_back(maze[(x + 1) * ROWS + y]);
  }
  if(!maze[x * ROWS + y + 1].isVisited() && y < COLUMNS - 1) {
    neighbours.push_back(maze[x * ROWS + y + 1]);
  }
  if(!maze[x * ROWS + y - 1].isVisited() && y > 0) {
    neighbours.push_back(maze[x * ROWS + y - 1]);
  }
  if (neighbours.size() < 1) {
    return -1;
  }
  
  int randomIdx = rand() % neighbours.size();
  int nxt = neighbours[randomIdx].getY() + neighbours[randomIdx].getX() * ROWS;
  return nxt;
}

void cap_framerate(Uint32 starting_tick) {
  if ( (1000 / FPS) > SDL_GetTicks() - starting_tick ) {
    SDL_Delay( 1000 / FPS - (SDL_GetTicks() - starting_tick) );
  } 
} 

int main(int argc, char *argv[]) {
  srand(time(NULL));
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);

  if (window == NULL) {
    std::cout << "There was an error initializing the window! :(\n" 
              << SDL_GetError();
    return 1;
  }

  Uint32 starting_tick;
  SDL_Event event;
  bool running = true;

  vector < Room > mazeGrid;
  stack < Room* > roomStack;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      Room newRoom(i, j, ROOM_WIDTH);
      mazeGrid.push_back(newRoom);
    }
  }

  Room *current = &mazeGrid[0]; 
  current->visit(); 

  while (running) {
    starting_tick = SDL_GetTicks();
    

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    int next = checkNeighbours(mazeGrid, *current);    
    // If the current cell has any neighbours which have not been visited
    if (next != -1) {
      // Choose randomly one of the unvisited neighbours
      Room &nextRoom = mazeGrid[next];
      // Push the current cell to the stack
      roomStack.push(current);
      // Remove the wall between the current cell and the chosen cell
      current->removeWalls(nextRoom);
      // Make the chosen cell the current cell and mark it as visited
      current = &nextRoom; 
      current->visit();
    } else if (!roomStack.empty()) { // If stack is not empty
      // Pop a cell from the stack
      Room &previousRoom = *roomStack.top();
      roomStack.pop();
      // Make it the current cell
      current = &previousRoom;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    for (Uint32 i = 0; i < mazeGrid.size(); i++) {
      if (!mazeGrid[i].isVisited()) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
      }
      SDL_Rect rect{mazeGrid[i].getX() * ROOM_WIDTH, mazeGrid[i].getY() * ROOM_WIDTH, ROOM_WIDTH, ROOM_WIDTH};
      SDL_RenderFillRect(renderer, &rect);
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
      mazeGrid[i].show(renderer);
    }
    
    SDL_SetRenderDrawColor(renderer, 55, 55, 55, SDL_ALPHA_OPAQUE);
    int xCoordHead = current->getX() * ROOM_WIDTH;
    int yCoordHead = current->getY() * ROOM_WIDTH;
    SDL_Rect rect{xCoordHead, yCoordHead, ROOM_WIDTH, ROOM_WIDTH};
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    cap_framerate(starting_tick);
  }


  SDL_DestroyWindow(window);  
  SDL_Quit();
  return 0;
}