// enemy.cpp
#include "enemy.h"
#include "background.h"
#include <iostream>

int ENEMY_SPEED=6;

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
    Enemy::init(tex, SCREEN_WIDTH, 440, 70, 70);
}

FlyingEnemy::FlyingEnemy() : frameIndex(0), animationDelay(0), width(0), height(0) {}

void FlyingEnemy::init(SDL_Texture* tex) {
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
    width /= 3;
    Enemy::init(tex, SCREEN_WIDTH, 350, width, height);
    destRect = {SCREEN_WIDTH, 350, width / 4, height / 4};
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
    flyingEnemyTex = IMG_LoadTexture(renderer, "images/enemy/flappybird.png");

    groundEnemy1.init(groundEnemyTex1);
    groundEnemy2.init(groundEnemyTex2);
    flyingEnemy.init(flyingEnemyTex);

    spawnRandomEnemy();
}

void EnemyManager::update() {
    groundEnemy1.update();
    groundEnemy2.update();
    flyingEnemy.update();
    if (groundEnemy2.isOffScreen()&&flyingEnemy.isOffScreen()) {
        spawnRandomEnemy();
    }
}

void EnemyManager::render(SDL_Renderer* renderer) {
    groundEnemy1.render(renderer);
    groundEnemy2.render(renderer);
    flyingEnemy.render(renderer);
}

void EnemyManager::spawnRandomEnemy() {

    int distance=SCREEN_WIDTH +rand()%301;
    //int randomType1 = rand() % 2;
    int randomType = rand() % 3;

    if (randomType != 0 ) {
        groundEnemy1.activate(SCREEN_WIDTH);
        groundEnemy2.activate(SCREEN_WIDTH+10+rand()%300);

    } else {
        flyingEnemy.activate(SCREEN_WIDTH);
    }
}

void EnemyManager::cleanUp() {
    SDL_DestroyTexture(groundEnemyTex1);
    SDL_DestroyTexture(groundEnemyTex2);
    SDL_DestroyTexture(flyingEnemyTex);
}
