#include <raylib.h>
#include "Game.hpp"
#include <string>

std::string formatWithLeadingZeros(int number, int width)
{
    std::string numberText=std::to_string(number);
    int leadingZero= width- numberText.length();
   return numberText=std::string(leadingZero, '0')+ numberText;
}

int main()
{
int windowWidth= 750;
int windowHeight=700;
Color grey= {29,29,27,255};
Color yellow= {243,216,63,255};
int offset=50;


InitWindow(windowWidth+offset,windowHeight+ 2*offset, "C++ SPACE INVADERS");
InitAudioDevice();
Font font= LoadFontEx("Font/monogram.ttf",64,0,0);
Texture2D spaceshipImage= LoadTexture("Graphics/spaceship.png");
SetTargetFPS(60);

Game game;

while(WindowShouldClose()==false)
{
    UpdateMusicStream(game.music);
    game.handleInput();
    game.update();
    BeginDrawing();
    ClearBackground(grey);
    DrawRectangleRoundedLines({10,10,780,780}, 0.18f,20,2,yellow);
    DrawLineEx({25,730},{775,730}, 3, yellow);
    if(game.run)
    DrawTextEx(font,"LEVEL 01",{570,740},34,2,yellow);
    else
    DrawTextEx(font,"GAME OVER",{570,740},34,2,yellow);
    float x= 50.0;
for(int i=0;i<game.lives;i++)
{
    DrawTextureV(spaceshipImage,{x,745},WHITE);
    x+=50;
}

    DrawTextEx(font,"SCORE",{50,15},34,2,yellow);
    std::string scoreText=formatWithLeadingZeros(game.score,5);
    DrawTextEx(font,scoreText.c_str(),{50,40},34,2,yellow);

    DrawTextEx(font,"HIGH-SCORE",{570,15},34,2,yellow);
    std::string highscoreText=formatWithLeadingZeros(game.highScore,5);
    DrawTextEx(font,highscoreText.c_str(),{655,40},34,2,yellow);

    game.draw();
    EndDrawing();

}

CloseWindow();
CloseAudioDevice();

}