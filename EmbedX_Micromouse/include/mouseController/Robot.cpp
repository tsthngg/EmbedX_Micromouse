#include "Robot.h"

Robot::Robot(int startX, int startY, int startDir) : x(startX), y(startY), dir(startDir) {
    
}
void Robot::rotateTo(int targetDir) {
    int diff = (targetDir - dir + 4) % 4; // độ lệch góc giữa góc quay muốn tới và góc hiện tại
    if (diff == 1) { 
        turnRight(); 
        dir = mod(dir + 1); 
    }
    else if (diff == 2) { 
        turnRight(); 
        turnRight(); 
        dir = mod(dir + 2); 
    }
    else if (diff == 3) { 
        turnLeft(); 
        dir = mod(dir + 3); 
    }
}
bool Robot::stepForwardAndSense(Maze& maze, bool visited[SIZE][SIZE]) {
    if (wallFront()) {
        maze.addWall(x, y, dir);
        return false;
    }
    moveForward();
    x += dx[dir];
    y += dy[dir];
    visited[x][y] = true;
    maze.senseWallsAndUpdate(x, y, dir);
    return true;
}
bool Robot::moveAlongPath(Maze& maze, const Path& path, bool visited[SIZE][SIZE]) {
    for (int idx = 0; idx < path.size(); idx++) {
        auto cell = path[idx];
        int tx = cell.first; 
        int ty = cell.second;
        if (exploredFlag && isCenter(x, y)) {
            return true;
        }
        // kiểm tra xem (tx, ty) và (rx, ry) có cạnh nhau không
        if (std::abs(tx - this->x) + std::abs(ty - this->y) != 1) { 
            return false;
        }

        int dfx = tx - x; 
        int dfy = ty - y;
        int wantDir = -1;

        for (int d = 0; d < 4; d++) {
            if (dfx == dx[d] && dfy == dy[d]) {
                wantDir = d; 
                break; 
            }
        }
        if (wantDir == -1) {
            return false;
        }

        // kiểm tra wall 
        auto it = (maze.walls).find({x, y});
        if (it != (maze.walls).end() && it->second.count(wantDir)) {
            return false;
        }
        // quay tới hướng cần đi
        rotateTo(wantDir);
        if (!stepForwardAndSense(maze, visited)) {
            return false;
        }
    }
    return true; 
}
void Robot::exploreFullMap(Maze& maze, bool visited[SIZE][SIZE]) {
    maze.senseWallsAndUpdate(x, y, dir);
    while (true) {
        bool moved = false;
        int order[4] = {-1, 0, 1, 2}; // ưu tiên: left, straight, right, back
        for (int i = 0; i < 4; i++) {
            int newdir = mod(this->dir + order[i]); // hướng mới của xe 
            int nx = x + dx[newdir]; 
            int ny = y + dy[newdir];
            if (!inRange(nx, ny)) {
                continue;
            }
            auto it = maze.walls.find({this->x, this->y});
            if (it != maze.walls.end() && it->second.count(newdir)) {
                continue;
            }
            if (!visited[nx][ny]) {
                rotateTo(newdir);
                bool canMove = stepForwardAndSense(maze, visited);
                if (!canMove) {
                    continue;
                }
                moved = true;
                break;
            }
        }

        if (moved) {
            continue;
        }
        // nếu không có ô kề chưa visit, tìm ô chưa visit gần nhất
        Path path = maze.bfsFindNearestUnvisited( x, y, visited);
        if (path.empty()) {
            // không còn ô  chưa visit, kết thúc explore
            break;
        }
        bool moveToUnvisitedCell = moveAlongPath( maze, path, visited);
        if (!moveToUnvisitedCell) {
            // nếu bị block khi đi theo path, lặp lại
            continue;
        }
    }
}
bool Robot::hasWall(Maze& maze) {
    auto it = maze.walls.find({x, y});
    if (it == maze.walls.end()) return false;
    return it->second.count(dir);
}
int Robot::getBestDirectionToGoal(Maze& maze, int floodOut[SIZE][SIZE]) {
    int bestDir = -1;
    for (int d = 0; d < 4; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if(inRange(nx, ny) && !hasWall(maze)) {
            if (floodOut[nx][ny] == (floodOut[x][y] - 1)) {
                bestDir = d;
                return bestDir;
            }
        }

    }
    return bestDir;
}
void Robot:: floodfillToGoal(Maze& maze, int floodOut[SIZE][SIZE]) {
    while(!isCenter(x,y)) {
        int nextDir = getBestDirectionToGoal(maze, floodOut);
        if(nextDir != -1) {
            rotateTo(nextDir);
            moveForward();
            x += dx[dir];
            y += dy[dir];
        }
    }
}
