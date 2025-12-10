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
    bool stepForwardAndSense(Maze& maze, bool visited[SIZE][SIZE]); // di chuyển vè phía trước 1 bước và cập nhật tường nếu có
    bool moveAlongPath(Maze& maze, const Path& path, bool visited[SIZE][SIZE]);// đi theo 1 path chứa các tọa độ
    void exploreFullMap(Maze& maze, bool visited[SIZE][SIZE]); // di chuyển full map và quét cập nhật bản đồ
    bool hasWall(Maze& maze); // kiếm tra có tường phía trước hay không
    int getBestDirectionToGoal(Maze& maze, int floodOut[SIZE][SIZE]); // tìm hướng có ô có trọng số nhỏ hơn ô hiện tại 1 đơn vị 
    void floodfillToGoal(Maze& maze, int floodOut[SIZE][SIZE]); // di chuyên từ ô hiện tại tới đích
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