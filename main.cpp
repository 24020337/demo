#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "background.h"
#include "enemy.h"
#include "character.h"


SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
const std::string WINDOW_TITLE = "SOS";
EnemyManager enemyManager;
Character character;

int score = 0;
int highScore;
Uint32 lastUpdateTime;
TTF_Font* font;
SDL_Color textColor;

void logSDLError(std::ostream& os, const std::string& msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}


void initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) logSDLError(std::cout, "CreateWindow", true);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "Linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w*0.57 && a.x + a.w*0.57 > b.x &&
            a.y < b.y + b.h*0.57 && a.y + a.h*0.57 > b.y);
}

void gameLoop() {
    bool running = true;
    SDL_Event e;
    int j=0;
    while (running) {
         while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym== SDLK_UP)) {
                character.jump();
            }
        }

        updateBackground();
        enemyManager.update();
        character.update();
        if (checkCollision(character.getRect(), enemyManager.groundEnemy1.destRect) ||
            checkCollision(character.getRect(), enemyManager.groundEnemy2.destRect) ||
            checkCollision(character.getRect(), enemyManager.flyingEnemy.destRect)) {
            running = false; // Dừng trò chơi khi có va chạm
        }

        SDL_RenderClear(renderer);
        renderBackground();
        character.render(renderer);
        enemyManager.render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(10);

    }
}

int main(int argc, char* argv[]) {
    initSDL(window, renderer);
    enemyManager.init(renderer);
    initBackgrounds();
    character.Init();


    gameLoop();


    closeBackgrounds();
    enemyManager.cleanUp();
    quitSDL(window, renderer);
    return 0;
}
