#pragma once
#include "spaceShip.hpp"
#include "Obstacle.hpp"
#include "Alien.hpp"
#include "MysteryShip.hpp"

class Game
{
    SpaceShip spaceShip;
    std::vector <Alien> aliens;
    int aliensDirection;
    std::vector <Laser> alienLasers;
    constexpr static float alienLaserShootInterval=0.35;
    float timeLastAlienFired;
    MysteryShip mysteryShip;
    float mysteryShipSpawnInterval;
    float timeLastSpawn;
    
     void deleteInactiveLasers();
     std::vector<Obstacle> obstacles;
     std::vector<Obstacle> createObstacles();
     std::vector <Alien> createAliens();
     void moveAliens();
     void moveDownAliens(int distance);
     void alienShootLaser();
     void checkForCollisions();
     void gameOver();
     void reset();
     void initGame();
     void checkForHighScore();
     void saveHighscoreToFile(int highscore);
     int loadHighscoreFromFile();
     Sound explosionSound;

    public:
    Game();
    ~Game();
    void draw();
    void update();
    void handleInput();
    bool run;
    int lives;
    int score;
    int highScore;
    Music music;
    
};