#include <Arduino.h>
#include "mouseController/Maze.h"
#include "mouseController/Robot.h"

Maze maze;
Robot robot(0, 0, 0);

bool visited[SIZE][SIZE];
int prevFlood[SIZE][SIZE];
int curFlood[SIZE][SIZE];

void setup(){
  memset(visited, 0, sizeof(visited));
  visited[0][0] = true;
}

void loop(){

 
}