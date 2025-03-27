#include "enemy.h"
#include "background.h"
#include <iostream>
#include "score.h"


// tăng tốc độ
float ENEMY_SPEED=5.0f;
float ENEMY_SPEED_MAX=12.0f;

Enemy::Enemy() : texture(nullptr), isActive(false) {}

void Enemy::init(SDL_Texture* tex, int x, int y, int w, int h) {
    texture = tex;
    srcRect = {0, 0, w, h};
    destRect = {x, y, w, h};
    isActive = false;
}

void Enemy::update() {
    if (!isActive) return;
    destRect.x -=ENEMY_SPEED;

    if (destRect.x < -destRect.w) {
        isActive = false;
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    if (isActive)
        SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Enemy::activate(int startX) {
    destRect.x = startX;
    isActive = true;
}

bool Enemy::isOffScreen() { return !isActive; }

GroundEnemy::GroundEnemy() {}
void GroundEnemy::init(SDL_Texture* tex) {
    Enemy::init(tex, SCREEN_WIDTH, 450, 60, 60);
}

FlyingEnemy::FlyingEnemy() : frameIndex(0), animationDelay(0), width(0), height(0) {}

void FlyingEnemy::init(SDL_Texture* tex) {
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
    width /= 3;
    Enemy::init(tex, SCREEN_WIDTH, 370, width, height);
    destRect = {SCREEN_WIDTH, 370, width / 4, height / 4};
    for (int i = 0; i < 3; i++) {
        frames[i] = {i * width, 0, width, height};
    }
}

void FlyingEnemy::update() {
    Enemy::update();
    if (isActive) {
        animationDelay++;
        if (animationDelay > 5) {
            frameIndex = (frameIndex + 1) % 3;
            animationDelay = 0;
        }
        srcRect = frames[frameIndex];
    }
}
void EnemyManager::init(SDL_Renderer* renderer) {
    groundEnemyTex1 = IMG_LoadTexture(renderer, "images/enemy/cactus.png");
    groundEnemyTex2 = IMG_LoadTexture(renderer, "images/enemy/cactus.png");
    groundEnemyTex3 = IMG_LoadTexture(renderer, "images/enemy/cactus.png");
    flyingEnemyTex  = IMG_LoadTexture(renderer, "images/enemy/flappybird.png");

    groundEnemy1.init(groundEnemyTex1);
    groundEnemy2.init(groundEnemyTex2);
    groundEnemy3.init(groundEnemyTex3);
    flyingEnemy.init(flyingEnemyTex);

    spawnRandomEnemy();
}

void EnemyManager::update() {
    groundEnemy1.update();
    groundEnemy2.update();
    groundEnemy3.update();
    flyingEnemy.update();
    if (groundEnemy3.isOffScreen()&&flyingEnemy.isOffScreen() && score<300) {
        spawnRandomEnemy();
    }
    if (groundEnemy3.isOffScreen()&&flyingEnemy.isOffScreen() && score>=310) {
        spawnRandomEnemy();
    }
}

void EnemyManager::render(SDL_Renderer* renderer) {
    groundEnemy1.render(renderer);
    groundEnemy2.render(renderer);
    groundEnemy3.render(renderer);
    flyingEnemy.render(renderer);
}

void EnemyManager::spawnRandomEnemy() {

    int distance=SCREEN_WIDTH +rand()%301;
    int randomType = rand() % 3;

    if (randomType != 0 ){
        if(score<300){
        groundEnemy1.activate(SCREEN_WIDTH);
        groundEnemy3.activate(SCREEN_WIDTH+300);
        }
        if(score>=310){
        groundEnemy1.activate(SCREEN_WIDTH);
        groundEnemy2.activate(SCREEN_WIDTH+300);
        groundEnemy3.activate(SCREEN_WIDTH+750);
        }
    }
    else {
        if(score<500){
        flyingEnemy.activate(SCREEN_WIDTH);
        }
        if(score>=510){
        flyingEnemy.activate(SCREEN_WIDTH);
        groundEnemy3.activate(SCREEN_WIDTH+50+rand()%50);
        }
    }
}

void EnemyManager::close() {
    SDL_DestroyTexture(groundEnemyTex1);
    SDL_DestroyTexture(groundEnemyTex2);
    SDL_DestroyTexture(flyingEnemyTex);
}

void EnemyManager::reset() {

    ENEMY_SPEED = 5.0f;

    groundEnemy1.destRect.x = 0;
    groundEnemy2.destRect.x = 0;
    groundEnemy3.destRect.x = 0;
    flyingEnemy.destRect.x  = 0;

    groundEnemy1.isActive = false;
    groundEnemy2.isActive = false;
    groundEnemy3.isActive = false;
    flyingEnemy.isActive  = false;
}
