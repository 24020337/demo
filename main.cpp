#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "background.h"
#include "enemy.h"

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
const std::string WINDOW_TITLE = "SOS";
EnemyManager enemyManager;

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

void gameLoop() {
    bool running = true;
    SDL_Event e;
    int j=0;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        updateBackground();
        enemyManager.update();

        SDL_RenderClear(renderer);
        renderBackground();
        enemyManager.render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // Giới hạn 60 FPS

    }
}

int main(int argc, char* argv[]) {
    initSDL(window, renderer);

    enemyManager.init(renderer);
    initBackgrounds();
    gameLoop();
    closeBackgrounds();
    enemyManager.cleanUp();
    quitSDL(window, renderer);
    return 0;
}
