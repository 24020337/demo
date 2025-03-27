#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "background.h"
#include "enemy.h"
#include "character.h"
#include "score.h"

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;
const std::string WINDOW_TITLE = "SOS";
EnemyManager enemyManager;
Character character;

// Score float để tính toán chính xác
float scoreF = 0.0f;

void logSDLError(std::ostream& os, const std::string& msg, bool fatal);
void initSDL(SDL_Window*& window, SDL_Renderer*& renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

bool isPlaying = false; // Khi false, hiển thị nút "PLAY"

SDL_Texture* playButton = NULL;
SDL_Texture* reset = NULL;
SDL_Rect playRect = {425, 225, 150, 98}; // Vị trí và kích thước của nút "PLAY"
SDL_Rect scrButton={0,0,150,98};

void loadPlayButton() {
    SDL_Surface* surface = IMG_Load("play_button.png");
    playButton = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}
void loadreset() {
    SDL_Surface* surface = IMG_Load("lose.png");
    reset = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Kiểm tra click vào nút PLAY
bool isPlayButtonClicked(int mouseX, int mouseY) {
    return (mouseX >= playRect.x && mouseX <= playRect.x + playRect.w &&
            mouseY >= playRect.y && mouseY <= playRect.y + playRect.h);
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w * 0.57 && a.x + a.w * 0.57 > b.x &&
            a.y < b.y + b.h * 0.57 && a.y + a.h * 0.57 > b.y);
}

void gameLoop() {
    SDL_Event e;
    Uint32 lastTime = SDL_GetTicks(), currentTime;
    float deltaTime;

    // Reset mọi thứ khi bắt đầu vòng chơi mới
    scoreF = 0;
    score = 0;
    enemyManager.reset();
    character.reset();
    isPlaying = true;  // Game bắt đầu chơi

    while (isPlaying) { // Chạy đến khi thua
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return; // Thoát game
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP)){
                Mix_Chunk* jumpSound = Mix_LoadWAV("sound/jump_sound.wav");
                Mix_PlayChannel(-1, jumpSound, 0);
                  character.jump();
            }
        }

        // Cập nhật thời gian trôi qua
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Cập nhật điểm số
        scoreF += deltaTime * 20;
        int newScore = (int)scoreF;
        if (newScore > score) {
            score = newScore;
            updateScoreTextures();
        }

        // Cập nhật tốc độ nếu điểm cao hơn
        if (score % 100 == 0) {
            ENEMY_SPEED += 0.1;
            if (ENEMY_SPEED > ENEMY_SPEED_MAX) ENEMY_SPEED = ENEMY_SPEED_MAX;
        }

        // Cập nhật nhân vật và kẻ địch
        updateBackground();
        enemyManager.update();
        character.update();

        // Kiểm tra va chạm
        if (checkCollision(character.getRect(), enemyManager.groundEnemy1.destRect) ||
            checkCollision(character.getRect(), enemyManager.groundEnemy2.destRect) ||
            checkCollision(character.getRect(), enemyManager.groundEnemy3.destRect) ||
            checkCollision(character.getRect(), enemyManager.flyingEnemy.destRect)) {

            if (score > highScore) {
                highScore = score;
                saveHighScore(highScore);
                updateScoreTextures();
            }

            isPlaying = false;  // Game over
            Mix_Chunk* loseSound = Mix_LoadWAV("sound/lose_sound.wav");
            Mix_PlayChannel(-1, loseSound, 0);
        }

        // Vẽ lại màn hình
        SDL_RenderClear(renderer);
        renderBackground();
        character.render(renderer);
        enemyManager.render(renderer);
        renderscore();
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* argv[]) {
    initSDL(window, renderer);
    enemyManager.init(renderer);
    initBackgrounds();
    character.Init();
    loadPlayButton();
    loadreset();
    loadHighScore();

    // Khởi tạo âm thanh
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Chunk* bgSound = Mix_LoadWAV("sound/bkgr_audio.wav");
    Mix_PlayChannel(-1, bgSound, -1);

    // Khởi tạo font chữ
    TTF_Init();
    font = TTF_OpenFont("font/pixel_font.ttf", 24);
    if (!font) logSDLError(std::cout, "TTF_OpenFont", true);

    SDL_Event event;
     bool waitingToStart = true;
        while (waitingToStart) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) return 0;

                if (event.type == SDL_MOUSEMOTION) {
                    int X = event.motion.x, Y = event.motion.y;
                    if(isPlayButtonClicked(X,Y)){
                    scrButton={150,0,150,98};
                }
                else{
                    scrButton={0,0,150,98};
                }

                }

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Mix_Chunk* clickSound = Mix_LoadWAV("sound/mouse_click.wav");
                    Mix_PlayChannel(-1, clickSound, 0);
                    if (isPlayButtonClicked(x, y)) {
                        waitingToStart = false;  // Bắt đầu chơi
                    }
                }
            }

            // Hiển thị nút "PLAY"
            SDL_RenderClear(renderer);
            renderBackground();
            SDL_RenderCopy(renderer, playButton, &scrButton, &playRect);
            SDL_RenderPresent(renderer);
        }

    while (true) {

        //START GAME
        gameLoop();

        bool waitingForRestart = true;
        while (waitingForRestart) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) return 0;
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) exit(0);
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                    waitingForRestart = false; // Restart game
                }
            }

            // Hiển thị màn hình thua
            SDL_RenderClear(renderer);
            renderBackground();
            character.render(renderer);
            enemyManager.render(renderer);
            renderscore();
            SDL_RenderCopy(renderer, reset, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

    // Giải phóng tài nguyên (nếu có thoát vòng lặp chính)
    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(reset);
    closeScore();
    closeBackgrounds();
    enemyManager.close();
    quitSDL(window, renderer);

    return 0;
}



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
