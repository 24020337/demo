#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "background.h"


class Character {
private:
    SDL_Texture* texture;
    SDL_Rect frames[6]; // 6 frames animation
    SDL_Rect destRect;
    int frameIndex;
    int animationDelay;
    float velocityY;
    float gravity;
    bool isJumping;

public:
    Character();
    void init(SDL_Texture* tex, int x, int y, int w, int h);
    void Init();
    void update() ;
    void render(SDL_Renderer* renderer);
    void jump() ;
    void reset();
    SDL_Rect getRect();
};


#endif // CHARACTER_H_INCLUDED
