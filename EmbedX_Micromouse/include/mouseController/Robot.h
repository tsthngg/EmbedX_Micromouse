#ifndef ROBOT_H
#define ROBOT_H
#include "Common.h"
#include "motorControl/motorControl.h"
#include "Maze.h"

class Robot {
private:
    int x; 
    int y; 
    int dir;
    bool exploredFlag = false;
    void rotateTo(int targetDir);
public:
    Robot(int startX = 0, int startY = 0, int startDir = 0);
    bool stepForwardAndSense(Maze& maze, bool visited[SIZE][SIZE]);
    bool moveAlongPath(Maze& maze, const Path& path, bool visited[SIZE][SIZE]);
    void exploreFullMap(Maze& maze, bool visited[SIZE][SIZE]);
    int getX()  { 
        return x; 
    }
    int getY()  { 
        return y; 
    }
    int getDir()  { 
        return dir; 
    }
    bool getExploredFlag() {
        return exploredFlag;
    }
    void setExploredFlag(bool mode) {
        exploredFlag = mode;
    }
};
#endif