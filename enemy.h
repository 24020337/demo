#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>
#include <cstdlib>
#include <ctime>
#include "background.h"


extern float ENEMY_SPEED_BASE;
extern float ENEMY_SPEED;
extern float ENEMY_SPEED_MAX;

class Enemy {
protected:
    SDL_Texture* texture;

public:
    SDL_Rect srcRect, destRect;
    int isActive;
    Enemy();
    void init(SDL_Texture* tex, int x, int y, int w, int h);
    virtual void update();
    void render(SDL_Renderer* renderer);
    void activate(int startX);
    bool isOffScreen();
};


class GroundEnemy : public Enemy {
public:
    GroundEnemy();
    void init(SDL_Texture* tex);
};

class FlyingEnemy : public Enemy {
private:
    SDL_Rect frames[3];
    int frameIndex;
    int animationDelay;
    int width, height;

public:
    FlyingEnemy();
    void init(SDL_Texture* tex);
    void update() override;
};

class EnemyManager {

public:
    SDL_Texture* groundEnemyTex1;
    SDL_Texture* groundEnemyTex2;
    SDL_Texture* groundEnemyTex3;
    SDL_Texture* flyingEnemyTex;
    GroundEnemy groundEnemy1,groundEnemy2,groundEnemy3;
    FlyingEnemy flyingEnemy;
    void setEnemySpeed(int speed);
    void init(SDL_Renderer* renderer);
    void update();
    void render(SDL_Renderer* renderer);
    void spawnRandomEnemy();
    void close();
};

#endif
