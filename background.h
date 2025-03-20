#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int GROUND_LEVEL = 440;
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 550;
const int LAYER_COUNT = 8;

extern SDL_Renderer* renderer;
extern SDL_Window* window;

void initBackgrounds();
void updateBackground();
void renderBackground();
SDL_Texture* LoadTexture(std::string path, SDL_Renderer* renderer);
void closeBackgrounds();

#endif
