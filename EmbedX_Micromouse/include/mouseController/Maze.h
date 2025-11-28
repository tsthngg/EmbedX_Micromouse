#ifndef MAZE_H
#define MAZE_H
#include "Common.h"
#include "irSensor/irSensor.h"
#include "motorControl/motorControl.h"

class Maze{ 
private:
    int flood[SIZE][SIZE];
public:
    Maze();
    Walls walls;
    void addWall (int x, int y, int dir);
    void senseWallsAndUpdate (int x, int y, int dir);
    void floodFill (int floodOut[SIZE][SIZE]);
    Path bfsFindPath (int sx, int sy, int tx, int ty);
    Path bfsFindNearestUnvisited (int sx, int sy, bool visited[SIZE][SIZE]);
    const Walls& getWalls() { 
        return walls; 
    }
};
#endif