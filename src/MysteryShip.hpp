#pragma once
#include <raylib.h>

class MysteryShip
{
    Vector2 position;
    Texture2D image;
    int speed;

public:
    MysteryShip();
    ~MysteryShip();
    void draw();
    void update();
    void spawn();
    Rectangle getRect();
    bool alive;
};