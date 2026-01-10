#include <Arduino.h>
#include "mouseController/Robot.h"
Robot robot(0, 0, 0);
Maze maze;
bool visited[SIZE][SIZE] = {false};
int curFlood[SIZE][SIZE] = {0};
char nextdir; // 'l', 'r', 'f', 'b', 's'
bool inMotion = false;
bool rotated = false;
long long setpoint;
float outputLeft;
float outputRight;
int dt;
int dirL;
int dirR;
PID pidLeft(0.141 ,0,0);//0.108
PID pidRight(0.125,0,0);//0.105850
long long currentTime = micros();
long long lastTime = 0;

void setup() {
  setupMotor_control();
  memset(visited, 0, sizeof(visited));
  visited[0][0] = true;
  maze.floodFill(curFlood);
}

void loop() {
  if (!inMotion) {
    lastTime = 0;
    if (!rotated) {
      maze.senseWallsAndUpdate(robot.getX(), robot.getY(), robot.getDir());
      int order[4] = {-1, 0, 1, 2}; // ưu tiên: left, straight, right, back
      for (int i = 0; i < 4; i++) {
        int newdir = mod(robot.getDir() + order[i]); // hướng mới của xe 
        int nx = robot.getX() + dx[newdir]; 
        int ny = robot.getY() + dy[newdir];
        if (!inRange(nx, ny)) {
            continue;
        }
        auto it = maze.walls.find({robot.getX(), robot.getY()});
        if (it != maze.walls.end() && it->second.count(newdir)) {
            continue;
        }
        if (!visited[nx][ny]) {
            
        }
      }
    }
    
  }

  if (nextdir == 'f') {
        setpoint = moveForward(setpoint);
        dirL = 1;
        dirR = 0;

  }
  else if (nextdir == 'l') {
        setpoint = turnLeft(setpoint);
        dirL = 1;
        dirR = 1;
  }
  else if (nextdir == 'r') {
        setpoint = turnRight(setpoint);
        dirL = 0;
        dirR = 0;
  }
  else if (nextdir == 'b') {
        setpoint = turnBack();
        dirL = 0;
        dirR = 0;
  }
  dt = currentTime - lastTime;
  lastTime = currentTime;
  
  outputLeft = Motor_controlLeft(setpoint, encoder1Value, dt);
  outputRight = Motor_controlRight(setpoint, encoder2Value, dt);
  motor_control(outputLeft, outputRight,dirL, dirR);
  if (outputLeft < 30){
    inMotion = false ;
  }
  
//
}