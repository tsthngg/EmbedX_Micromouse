#ifndef COMMON_H
#define COMMON_H
#include <vector>
#include <utility>
#include <unordered_map>
#include <set>
#include <algorithm>
const int SIZE = 16; // kích thước bản đồ
extern const int dx[4];
extern const int dy[4];
int mod(int x);
bool inRange(int x, int y);
bool isCenter(int x, int y);
struct pair_hash {
    size_t operator()(const std::pair<int,int> &p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};
struct Cell
{
    int x;
    int y;
};

using Walls = std::unordered_map<std::pair<int,int>, std::set<int>, pair_hash>;
using Path = std::vector<std::pair<int,int>>;

#endif