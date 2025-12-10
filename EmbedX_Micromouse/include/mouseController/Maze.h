#ifndef MAZE_H
#define MAZE_H
#include "Common.h"
#include "irSensor/irSensor.h"
#include "motorControl/motorControl.h"

class Maze{ 
private:
    
public:
    Maze();
    Walls walls;
    void addWall (int x, int y, int dir); // thêm tường
    void senseWallsAndUpdate (int x, int y, int dir); // kiểm tra tường xung quanh
    void floodFill (int floodOut[SIZE][SIZE]); // lan bfs từ trong tâm tính trọng số các ô
    Path bfsFindPath (int sx, int sy, int tx, int ty); // lan bfs trà về path chứa tọa độ thứ tự các ô đi từ (sx,sy) ->(tx,ty)
    Path bfsFindNearestUnvisited (int sx, int sy, bool visited[SIZE][SIZE]);// tìm ô chưa quét gần nhất
    const Walls& getWalls() { 
        return walls; 
    }
};
#endif