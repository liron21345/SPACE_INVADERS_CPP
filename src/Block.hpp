#pragma once
#include <raylib.h>

class Block{
Vector2 position;

public:
Block(Vector2 position);
void draw();
Rectangle getRect();


};