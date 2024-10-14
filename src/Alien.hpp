#pragma once
#include <raylib.h>

class Alien
{

public:
static Texture2D alienImages[3];
int type;
Vector2 position;
Alien(int type, Vector2 position);
void update(int direction);
void draw();
int getType();
static void unloadImages();
Rectangle getRect();
};