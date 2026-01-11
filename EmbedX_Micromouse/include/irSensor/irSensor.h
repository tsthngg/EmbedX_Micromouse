#ifndef IRSENSOR_H

#define IRSENSOR_H

const int irFrontLeftPin = 36;
const int irFrontRightPin = 34;
const int irUpperLeftPin = 39;
const int irUpperRightPin = 35;
const int irLowerLeftPin = 14;
const int irLowerRightPin = 15;

bool checkWall(char side);

#endif