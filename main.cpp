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

// High Score
int score = 0;
int highScore = 0;
TTF_Font* font;
SDL_Color textColor = {50, 50, 50  };
SDL_Texture* scoreTexture = NULL;
SDL_Texture* highScoreTexture = NULL;
SDL_Rect scoreRect = {800, 20, 100, 30};
SDL_Rect highScoreRect = {800, 50, 150, 30};

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

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

void saveHighScore(int highScore) {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

int loadHighScore() {
    int hs = 0;
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> hs;
        file.close();
    }
    return hs;
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w * 0.57 && a.x + a.w * 0.57 > b.x &&
            a.y < b.y + b.h * 0.57 && a.y + a.h * 0.57 > b.y);
}

void updateScoreTextures() {
    if (scoreTexture) SDL_DestroyTexture(scoreTexture);
    if (highScoreTexture) SDL_DestroyTexture(highScoreTexture);

    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, ("Score: " + std::to_string(score)).c_str(), textColor);
    scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    scoreRect = {820, 50, scoreSurface->w, scoreSurface->h};
    SDL_FreeSurface(scoreSurface);

    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, ("High Score: " + std::to_string(highScore)).c_str(), textColor);
    highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    highScoreRect = {780, 20, highScoreSurface->w, highScoreSurface->h};
    SDL_FreeSurface(highScoreSurface);
}

void gameLoop() {
    bool running = true;
    SDL_Event e;
    Uint32 lastTime = SDL_GetTicks(), currentTime;
    float deltaTime;
    highScore = loadHighScore();
    updateScoreTextures();

    while (running) {

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP)) {
                character.jump();
            }
        }

        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        score += (int)(deltaTime * 50);  // Cộng điểm theo thời gian

        updateBackground();
        enemyManager.update();
        character.update();

        if (checkCollision(character.getRect(), enemyManager.groundEnemy1.destRect) ||
            checkCollision(character.getRect(), enemyManager.groundEnemy2.destRect) ||
            checkCollision(character.getRect(), enemyManager.flyingEnemy.destRect)) {
            if (score > highScore) {
                highScore = score;
                saveHighScore(highScore);
            }
            running = false;
        }

        updateScoreTextures();
        SDL_RenderClear(renderer);

        renderBackground();
        character.render(renderer);
        enemyManager.render(renderer);

        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreRect);

        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* argv[]) {
    initSDL(window, renderer);
    enemyManager.init(renderer);
    initBackgrounds();
    character.Init();

    TTF_Init();
    font = TTF_OpenFont("font/pixel_font.ttf", 24);
    if (!font) logSDLError(std::cout, "TTF_OpenFont", true);

    gameLoop();

    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(highScoreTexture);
    TTF_CloseFont(font);
    TTF_Quit();

    closeBackgrounds();
    enemyManager.cleanUp();
    quitSDL(window, renderer);
    return 0;
}
