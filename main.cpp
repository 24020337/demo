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
#include "states.h"

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
bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return (a.x < b.x + b.w * 0.57 && a.x + a.w * 0.57 > b.x &&
            a.y < b.y + b.h * 0.57 && a.y + a.h * 0.57 > b.y);
}

void gameLoop() {
    SDL_Event e;
    Uint32 lastTime = SDL_GetTicks(), currentTime;
    float deltaTime;

    // Reset trạng thái game khi bắt đầu vòng mới
    scoreF = 0;
    score = 0;
    enemyManager.reset();
    character.reset();
    isPlaying = true;
    bool isPaused = false; // Trạng thái tạm dừng

    while (isPlaying) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return; // Thoát game khi đóng cửa sổ
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) return; // Nhấn ESC để thoát game

                if (e.key.keysym.sym == SDLK_p) {
                    isPaused = !isPaused; // Nhấn P để tạm dừng hoặc tiếp tục
                }

                if (!isPaused && (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP)) {
                    Mix_Chunk* jumpSound = Mix_LoadWAV("sound/jump_sound.wav");
                    Mix_PlayChannel(-1, jumpSound, 0);
                    character.jump();
                }
            }
                if (e.type == SDL_QUIT) return ;

            if (e.type == SDL_MOUSEMOTION) {
                int X = e.motion.x, Y = e.motion.y;
                if(isButtonClicked(X,Y,pause_or_continue)){
                scrButton_PC.x=22;
                }
                else{
                scrButton_PC.x=0;
                }
            }

            // Kiểm tra click chuột vào nút Pause/Continue
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isButtonClicked(x, y, pause_or_continue)) {
                    isPaused = !isPaused;
                    Mix_Chunk* clickSound = Mix_LoadWAV("sound/mouse_click.wav");
                    Mix_PlayChannel(-1, clickSound, 0);
                }
            }
        }

        if (isPaused) {
            // Hiển thị màn hình "PAUSE"
            SDL_RenderClear(renderer);
            renderBackground();
            character.render(renderer);
            enemyManager.render(renderer);
            renderscore();
            SDL_RenderCopy(renderer, continueButton, &scrButton_PC, &pause_or_continue);
            SDL_RenderPresent(renderer);
            continue; // Bỏ qua các cập nhật game khi tạm dừng
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

        // Cập nhật tốc độ nếu điểm đạt mốc
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
        SDL_RenderCopy(renderer, pauseButton, &scrButton_PC, &pause_or_continue);
        SDL_RenderPresent(renderer);
    }
}

int main(int argc, char* argv[]) {
    initSDL(window, renderer);
    enemyManager.init(renderer);
    initBackgrounds();
    character.Init();
    load_All_Button();
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
                if(isButtonClicked(X,Y,playRect_Play)){
                scrButton_Play={150,0,150,98};
                }
                else{
                scrButton_Play={0,0,150,98};
                }

                if(isButtonClicked(X,Y,playRect_Exit)){
                scrButton_Exit={150,0,150,98};
                }
                else{
                scrButton_Exit={0,0,150,98};
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                Mix_Chunk* clickSound = Mix_LoadWAV("sound/mouse_click.wav");
                Mix_PlayChannel(-1, clickSound, 0);

                if (isButtonClicked(x, y,playRect_Play)) {
                    waitingToStart = false;  // Bắt đầu chơi
                    }
                if (isButtonClicked(x, y,playRect_Exit)) {
                        SDL_Delay(240);     // Esc game
                        exit(0);
                    }
                }
            }

            // Hiển thị nút
            SDL_RenderClear(renderer);
            renderBackground();
            SDL_RenderCopy(renderer, playButton, &scrButton_Play, &playRect_Play);
            SDL_RenderCopy(renderer, exitButton, &scrButton_Exit, &playRect_Exit);
            SDL_RenderPresent(renderer);
        }

    while (true) {

        //START GAME
        gameLoop();
        // đợi cho đến khi chơi lại
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
