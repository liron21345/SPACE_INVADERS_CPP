#include "SpaceShip.hpp"


SpaceShip::SpaceShip()
{
    image= LoadTexture("Graphics/spaceship.png");
    position.x=((GetScreenWidth()-image.width)/2);
    position.y=(GetScreenHeight()-image.height)-100;
    lastFireTime=0.0;
    laserSound=LoadSound("Sounds/laser.ogg");
}

SpaceShip::~SpaceShip()
{
    UnloadTexture(image);
    UnloadSound(laserSound);
}

void SpaceShip::draw()
{
    DrawTextureV(image,position,WHITE);
}

void SpaceShip::moveLeft()
{
    position.x-=7;
    if(position.x<25)
    position.x=25;
}

void SpaceShip::moveRight()
{
    position.x+=7;
    if(position.x>GetScreenWidth()-image.width-25)
    position.x=GetScreenWidth()-image.width-25;
}

void SpaceShip::fireLaser()
{
    if (GetTime()-lastFireTime>0.35)
    {
    lasers.push_back(Laser({position.x+image.width/2-2,position.y}, -6));
    lastFireTime=GetTime();
    PlaySound(laserSound);
    }
}

Rectangle SpaceShip::getRect()
{
    return {position.x, position.y, float(image.width), float(image.height)};
}

void SpaceShip::reset()
{
    position.x=(GetScreenWidth()-image.width)/2.0f;
    position.y=GetScreenHeight()-image.height-100;
    lasers.clear();
}
