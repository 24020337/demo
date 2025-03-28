#ifndef STATES_H_INCLUDED
#define STATES_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

extern SDL_Event event;
extern SDL_Texture* playButton;
extern SDL_Texture* exitButton;
extern SDL_Texture* pauseButton;
extern SDL_Texture* continueButton;
extern SDL_Texture* reset;
//extern SDL_Rect playRect ;
extern SDL_Rect scrButton;

extern SDL_Rect playRect_Play ;
extern SDL_Rect scrButton_Play;

extern SDL_Rect playRect_Exit;
extern SDL_Rect scrButton_Exit;

extern SDL_Rect pause_or_continue ;
extern SDL_Rect scrButton_PC;

void load_Play_Button();
void load_Exit_Button();
void load_Continue_Button();
void load_Pause_Button();
void load_All_Button();
void loadreset();
bool isButtonClicked(int mouseX, int mouseY,SDL_Rect button);


#endif // STATES_H_INCLUDED
