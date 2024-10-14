#pragma once
#include <raylib.h>

class Laser{
int speed;
Vector2 position;

public:
bool active;
Laser(Vector2 position, int speed);
void update();
void draw();
Rectangle getRect();

};