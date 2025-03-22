#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include <SDL_ttf.h>
#include <fstream>
#include <iostream>
#include <SDL.h>

extern int score;
extern int highScore;
extern TTF_Font* font;
extern SDL_Color textColor;
extern SDL_Texture* scoreTexture;
extern SDL_Texture* highScoreTexture;
extern SDL_Rect scoreRect;
extern SDL_Rect highScoreRect;

void saveHighScore(int highScore);
int loadHighScore();
void updateScoreTextures();
void renderscore();
void closeScore();

#endif // SCORE_H_INCLUDED
