#ifndef STATES_H_INCLUDED
#define STATES_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

SDL_Texture* playButton;
SDL_Texture* reset;
SDL_Rect playRect_Play = {425, 125, 150, 98};
SDL_Rect scrButton_Play_notClick={0,0,150,98};
SDL_Rect scrButton_Play_Click={150,0,150,98};

SDL_Rect playRect_Exit = {425, 325, 150, 98};
SDL_Rect scrButton_Exit_notClick={0,0,150,98};
SDL_Rect scrButton_Exit_Click={150,0,150,98};

SDL_Rect pause_or_continue = {10,10,22,34};

SDL_Rect scrButton_Pause_notClick = {0,0,22,34};
SDL_Rect scrButton_Pause_Click = {10,0,22,34};

SDL_Rect scrButton_Play_notClick = {0,0,22,34};
SDL_Rect scrButton_Play_Click = {10,0,22,34};

void loadButton();
void    loadreset();
bool isPlayButtonClicked(int mouseX, int mouseY,SDL_Rect button);



#endif // STATES_H_INCLUDED
