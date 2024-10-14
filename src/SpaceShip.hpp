#pragma once
#include <raylib.h>
#include "Laser.hpp"
#include <vector>

class SpaceShip
{
    Texture2D image;
    Vector2 position;
    double lastFireTime;
    Sound laserSound;

public:
SpaceShip();
~SpaceShip();
void draw();
void moveLeft();
void moveRight();
void fireLaser();
std::vector<Laser> lasers;
Rectangle getRect();
void reset();
};