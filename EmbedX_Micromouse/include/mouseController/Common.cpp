#include "mouseController/Common.h"
// các hướng 
// 0 = Bắc, 1 = Đông, 2 = Nam, 3 = Tây
// hướng 0 (Bắc):   dx = 0,  dy = 1   đi lên
// hướng 1 (Đông):  dx = 1, dy = 0    đi sang phải
// hướng 2 (Nam):   dx = 0,  dy = -1  đi xuống
// hướng 3 (Tây):   dx = -1, dy = 0   đi sang trái
const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {1, 0, -1, 0};

int mod(int x) { 
    return (x + 4) % 4; 
}

bool inRange(int x, int y) {
    return x >= 0 && y >= 0 && x < SIZE && y < SIZE;
}

bool isCenter(int x, int y) {
    return (x == 7 || x == 8) && (y == 7 || y == 8);
}