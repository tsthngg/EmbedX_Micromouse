#include "Maze.h"
#include <queue>
#include <algorithm>

Maze::Maze() {
    for (int i = 0; i < SIZE; i++) {
        addWall(i, 0, 2);
        addWall(i, SIZE-1, 0);
        addWall(0, i, 3);
        addWall(SIZE-1, i, 1);
    }
}
void Maze::addWall(int x, int y, int dir) {
    walls[{x,y}].insert(dir);
    int nx = x + dx[dir]; // newx
    int ny = y + dy[dir]; // newy
    if (inRange(nx, ny)) {
        walls[{nx, ny}].insert(mod(dir + 2)); // nếu có tường ở 1 ô thì thêm tường ở ô liền kề
    }
}
void Maze::senseWallsAndUpdate(int x, int y, int dir) {
    if (checkWall('f'))  addWall(x, y, dir);
    if (checkWall('l'))   addWall(x, y, mod(dir - 1));
    if (checkWall('r'))  addWall(x, y, mod(dir + 1));
}
void Maze::floodFill(int floodOut[SIZE][SIZE]) {
    std::queue<Cell> q;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            floodOut[i][j] = 255; 
        }
    }
    int cx[4] = {7,7,8,8}; // center x
    int cy[4] = {7,8,7,8}; // center y
    for (int k = 0; k < 4; k++) {
        floodOut[cx[k]][cy[k]] = 0;
        q.push({cx[k], cy[k]});
    }
    while (!q.empty()) {
        Cell cur = q.front(); 
        q.pop();
        int d = floodOut[cur.x][cur.y];
        for (int dir = 0; dir < 4; dir++) {
            int nx = cur.x + dx[dir]; // new x
            int ny = cur.y + dy[dir]; // new y
            if (!inRange(nx, ny)) {
                continue;
            }
            auto it = walls.find({cur.x, cur.y});
            if (it != walls.end() && it->second.count(dir)) {
                continue;
            } // nếu có tường thì không lan
            if (floodOut[nx][ny] > d + 1) {
                // nếu trọng số (nx, ny) lớn hơn d + 1 thì ta sẽ đi đến ô (x, y) trước tốn d bước 
                // sau đó đến (nx,ny) tốn thêm 1 bước
                floodOut[nx][ny] = d + 1; 
                q.push({nx, ny});
            }
        }
    }
}
Path Maze::bfsFindPath(int sx, int sy, int tx, int ty) {
    Path empty; // trả về khi không tìm thấy để tránh báo lỗi

    std::queue<std::pair<int,int>> q;
    bool seen[SIZE][SIZE] = {}; // đánh dấu các ô đã xử lí 
    std::pair<int,int> parent[SIZE][SIZE]; // đi từ ô đó đến ô (x, y) ( node cha)

    q.push({sx, sy});
    seen[sx][sy] = true;
    parent[sx][sy] = {-1, -1}; // không có ô nào nối với (sx, sy)

    bool found = false;
    while (!q.empty()) {
        auto cur = q.front(); 
        q.pop();
        int x = cur.first;
        int y = cur.second;
        if (x == tx && y == ty) { 
            found = true; 
            break; 
        }
        for (int d = 0; d < 4; ++d) { //  lan bfs ra 4 hướng
            int nx = x + dx[d];
            int ny = y + dy[d];
            if (!inRange(nx, ny)) {
                continue;
            }; // kiểm tra (nx, ny ) ngoài phạm vi thì không lan bfs
            auto it = walls.find({x, y});
            if (it != walls.end() && it->second.count(d)) {
                continue;
            }; // nếu có tường thì không lan bfs
            if (!seen[nx][ny]) {
                seen[nx][ny] = true; // đánh dấu để không nối (x, y) với 1 ô khác nữa
                parent[nx][ny] = {x, y}; // gán ô (x, y) với ô liền kề (nx, ny)
                q.push({nx, ny});
            }
        }
    }

    if (!found) return empty; 

    Path path; // path lưu trữ đường đi
    int cx = tx; 
    int cy = ty;
    while (!(cx == sx && cy == sy)) { // truy xuất ngược từ center về start
        path.push_back({cx, cy});
        auto p = parent[cx][cy];
        cx = p.first; 
        cy = p.second;
        if (cx == -1 && cy == -1) {
            break;
        } 
    }
    std::reverse(path.begin(), path.end()); // đảo ngược lại path để đi
    return path;
}
Path Maze::bfsFindNearestUnvisited (int sx, int sy, bool visited[SIZE][SIZE]) {
    Path empty;
    if (!inRange(sx, sy)) return empty;

    std::queue<std::pair<int,int>> q;
    bool seen[SIZE][SIZE] = {}; // đánh dấu các ô đã xử lí
    std::pair<int,int> parent[SIZE][SIZE]; // node cha 

    q.push({sx, sy});
    seen[sx][sy] = true;
    parent[sx][sy] = {-1, -1};

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        int x = cur.first, y = cur.second;

        if (!visited[x][y]) {
            Path path;
            int cx = x, cy = y;
            while (!(cx == sx && cy == sy)) { // truy xuất ngược
                path.push_back({cx, cy});
                auto p = parent[cx][cy];
                cx = p.first; 
                cy = p.second;
                if (cx == -1 && cy == -1) {
                    break;
                } 
            }
            std::reverse(path.begin(), path.end()); // đảo lại path
            return path;
        }

        for (int d = 0; d < 4; d++) { // kiểm tra 4 hướng
            int nx = x + dx[d]; 
            int ny = y + dy[d];
            if (!inRange(nx, ny)) continue; // (nx, ny) vượt ngoài map thì bỏ qua
            auto it = walls.find({x, y});
            if (it != walls.end() && it->second.count(d)) continue; // nếu có tường thì bỏ qua
            if (!seen[nx][ny]) {
                seen[nx][ny] = true;
                parent[nx][ny] = {x, y};
                q.push({nx, ny});
            }
        }
    }
    return empty;
} 