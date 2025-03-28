#include "states.h"
#include "background.h"
#include <SDL.h>
#include <SDL_image.h>
#include "enemy.h"
#include "character.h"
#include "score.h"
#include "states.h"
#include <SDL_mixer.h>


SDL_Texture* playButton;
SDL_Rect playRect_Play = {425, 200, 150, 98};
SDL_Rect scrButton_Play={0,0,150,98};

SDL_Texture* exitButton;
SDL_Rect playRect_Exit = {425, 300, 150, 98};
SDL_Rect scrButton_Exit={0,0,150,98};

SDL_Texture* pauseButton;
SDL_Texture* continueButton;
SDL_Rect pause_or_continue = {10,10,22,34};
SDL_Rect scrButton_PC = {0,0,22,34};

SDL_Texture* reset;

void load_Play_Button() {
    SDL_Surface* surface = IMG_Load("images/states/play_button.png");
    playButton = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void load_Exit_Button() {
    SDL_Surface* surface = IMG_Load("images/states/exit_button.png");
    exitButton = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void load_Pause_Button() {
    SDL_Surface* surface = IMG_Load("images/states/pause_button.png");
    pauseButton = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void load_Continue_Button() {
    SDL_Surface* surface = IMG_Load("images/states/continue_button.png");
    continueButton = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void loadreset() {
    SDL_Surface* surface = IMG_Load("images/states/lose.png");
    reset = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void load_All_Button()
{
    load_Play_Button();
    load_Continue_Button();
    load_Exit_Button();
    load_Pause_Button();

}

bool isButtonClicked(int mouseX, int mouseY,SDL_Rect playRect) {
    return (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
            mouseY >= playRect.y && mouseY <= playRect.y + playRect.h);
}


