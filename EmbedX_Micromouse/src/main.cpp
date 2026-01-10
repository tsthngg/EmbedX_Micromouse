#include <Arduino.h>
#include "mouseController/Robot.h"
Robot robot(0, 0, 0);
Maze maze;
bool visited[SIZE][SIZE] = {false};
int curFlood[SIZE][SIZE] = {0};
void setup() {
  memset(visited, 0, sizeof(visited));
  visited[0][0] = true;
  maze.floodFill(curFlood);
}

void loop() {
  robot.exploreFullMap(maze, visited);
  maze.floodFill(curFlood);
  robot.floodfillToGoal(maze, curFlood);
}