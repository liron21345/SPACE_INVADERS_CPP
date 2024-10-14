#include "Game.hpp"
#include <iostream>
#include <fstream>

Game::Game()
{
    music=LoadMusicStream("Sounds/music.ogg");
    explosionSound=LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    initGame();
}

Game::~Game()
{
    Alien::unloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

void Game::deleteInactiveLasers()
{
    for(auto it=spaceShip.lasers.begin();it!=spaceShip.lasers.end();)
    {
        if (!it->active)
        it=spaceShip.lasers.erase(it);
        else 
        it++;
    }

        for(auto it=alienLasers.begin();it!=alienLasers.end();)
    {
        if (!it->active)
        it=alienLasers.erase(it);
        else 
        it++;
    }
}

std::vector<Obstacle> Game::createObstacles()
{
  int obstacleWidth=Obstacle::grid[0].size()*3;
  float gap= (GetScreenWidth()-(obstacleWidth*4))/5;

  for(int i=0; i<4; i++)
  {
    float offsetX= (i+1)*gap+i*obstacleWidth;
    obstacles.push_back(Obstacle({offsetX,float(GetScreenHeight()-200)}));
  }
  return obstacles;

}

std::vector<Alien> Game::createAliens()
{
    std::vector<Alien> aliens;
    for(int row=0; row<5; row++)
    for (int col=0; col<11; col++)
    {
        int alienType;
        if(row==0)
        alienType=3;
        else if (row==1 ||row==2)
        alienType=2;
        else 
        alienType=1;
        float x=75+col * 55;
        float y= 110+ row * 55;
        aliens.push_back(Alien(alienType,{x,y}));
    }
    return aliens;
}

void Game::moveAliens()
{
    for(auto& alien:aliens)
    {
        if(alien.position.x+alien.alienImages[alien.type-1].width > GetScreenWidth()-25)
        {
           aliensDirection=-1;
           moveDownAliens(4);
        }
        if(alien.position.x<25)
        {
           aliensDirection=1;
           moveDownAliens(4);
        }
        alien.update(aliensDirection);
    }
}

void Game::moveDownAliens(int distance)
{
    for (auto& alien: aliens)
    alien.position.y+=distance;
}

void Game::alienShootLaser()
{
    double currentTime=GetTime();
    if (currentTime-timeLastAlienFired>=alienLaserShootInterval && !aliens.empty())
    {
    int randomIndex=GetRandomValue(0, aliens.size()-1);
    Alien& alien=aliens[randomIndex];
    alienLasers.push_back(Laser({alien.position.x+alien.alienImages[alien.type-1].width/2,
                                alien.position.y+ alien.alienImages[alien.type-1].height},6));
    
    timeLastAlienFired=GetTime();
    }
}

void Game::checkForCollisions()
{
    //spaceship laser

    for (auto& laser: spaceShip.lasers) 
    {
        auto it=aliens.begin();
        while(it!=aliens.end())
        {
            if (CheckCollisionRecs(it->getRect(), laser.getRect()))
            {
                PlaySound(explosionSound);
                if (it->type==1)
                score+=100;
                else if (it->type==2)
                score+=200;
                else if (it->type==3)
                score+=300;

                checkForHighScore();

                it=aliens.erase(it);
                laser.active=false;
            }
            else ++it;
        }

        for (auto& obstacle: obstacles)
        {
            auto it= obstacle.blocks.begin();
            while (it!=obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it->getRect(),laser.getRect()))
                {
                    it=obstacle.blocks.erase(it);
                    laser.active=false;
                }
                else ++it;
            }
        }
        if (CheckCollisionRecs(mysteryShip.getRect(),laser.getRect()))
        {
            PlaySound(explosionSound);
            mysteryShip.alive=false;
            laser.active=false;
            score+=500;
            checkForHighScore();
        }
        
    }

    //aliens laser

    for (auto& laser:alienLasers)
    {
        if(CheckCollisionRecs(laser.getRect(),spaceShip.getRect()))
        {
            laser.active=false;
            lives--;
            if(lives==0)
            gameOver();
        }

        for (auto& obstacle: obstacles)
        {
            auto it= obstacle.blocks.begin();
            while (it!=obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it->getRect(),laser.getRect()))
                {
                    it=obstacle.blocks.erase(it);
                    laser.active=false;
                }
                else ++it;
            }
        }
    }

    //aliens collision with obstacle

    for (auto& alien:aliens)
    {
        for(auto& obstacle:obstacles)
        {
            auto it=obstacle.blocks.begin();
            while (it!=obstacle.blocks.end())
            {
                if(CheckCollisionRecs(it->getRect(),alien.getRect()))
                {
                    it=obstacle.blocks.erase(it);
                }
                else ++it;
            }
        }
        if (CheckCollisionRecs(alien.getRect(),spaceShip.getRect()))
        {
            gameOver();
        }
    }
}

void Game::gameOver()
{
    run=false;
}

void Game::reset()
{
    spaceShip.reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();

}

void Game::initGame()
{
   obstacles=createObstacles();
    aliens=createAliens();
    aliensDirection=1;
    timeLastAlienFired=0.0;
    timeLastSpawn=0.0;
    mysteryShipSpawnInterval=GetRandomValue(10,20);
    lives=3;
    run=true;
    score=0;
    highScore=loadHighscoreFromFile();
}

void Game::checkForHighScore()
{
    if (score>highScore)
    {
        highScore=score;
        saveHighscoreToFile(highScore);
    }
}

void Game::saveHighscoreToFile(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open())
    {
        highscoreFile<< highscore;
        highscoreFile.close();
    }
    else
    {
        std::cerr<< "failed to save highscore to file"<< std::endl;
    }
}

int Game::loadHighscoreFromFile()
{
    int loadedHighscore=0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open())
    {
        highscoreFile>> loadedHighscore;
        highscoreFile.close();
    }
    else
    {
        std::cerr << "failed to load highscore from file" << std::endl;
    }
    return loadedHighscore;
}

void Game::draw()
{
    spaceShip.draw();

    for (auto& laser: spaceShip.lasers)
    {
        laser.draw();
    }

    for(auto& obstacle:obstacles)
    obstacle.draw();

    for (auto& alien:aliens)
    alien.draw();

    for (auto& laser:alienLasers)
    laser.draw();

    mysteryShip.draw();
}

void Game::update()
{
    if(run)
    {
        double currentTime=GetTime();
        if(currentTime-timeLastSpawn>mysteryShipSpawnInterval)
        {
        mysteryShip.spawn();
        timeLastSpawn=GetTime();
        mysteryShipSpawnInterval=GetRandomValue(10,20);
        }

        for(auto& laser: spaceShip.lasers)
        laser.update(); 
        moveAliens();
        alienShootLaser();
        for (auto& laser:alienLasers)
        laser.update();
        deleteInactiveLasers();
        mysteryShip.update();
        checkForCollisions();
    }
    else
    {
        if(IsKeyDown(KEY_ENTER))
        {
            reset();
            initGame();
        }
    }
}

void Game::handleInput()
{
    if(run)
    {
        if(IsKeyDown(KEY_LEFT))
        spaceShip.moveLeft();
        else if(IsKeyDown(KEY_RIGHT))
        spaceShip.moveRight();
        else if (IsKeyDown(KEY_SPACE))
        spaceShip.fireLaser();
    }
}