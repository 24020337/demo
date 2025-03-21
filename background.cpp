#include "background.h"

SDL_Texture* layers[LAYER_COUNT];
float layer_speed[LAYER_COUNT] = {0.1, 0.2, 0.3, 0.5, 0.5, 1.5, 1.5, 3};
float layer_cut[LAYER_COUNT] = {0, 0, 0, 0, 0, 30, 30, 0};
float bgX1[LAYER_COUNT], bgX2[LAYER_COUNT];

const char* layer_file[LAYER_COUNT] = {
    "images/background/layer01.png",
    "images/background/layer02.png",
    "images/background/layer03.png",
    "images/background/layer04.png",
    "images/background/layer05.png",
    "images/background/layer06.png",
    "images/background/layer07.png",
    "images/background/layer08.png"
};

SDL_Texture* LoadTexture(std::string path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadSurface = IMG_Load(path.c_str());
    if (!loadSurface) {
        std::cerr << "Failed to load image: " << path << " Error: " << IMG_GetError() << std::endl;
        return NULL;
    }
    newTexture = SDL_CreateTextureFromSurface(renderer, loadSurface);
    SDL_FreeSurface(loadSurface);
    return newTexture;
}

void initBackgrounds() {
    for (int i = 0; i < LAYER_COUNT; i++) {
        layers[i] = LoadTexture(layer_file[i], renderer);
        if (!layers[i]) {
            std::cerr << "Failed to load background layer: " << layer_file[i] << std::endl;
        }
        bgX1[i] = 0;
        bgX2[i] = SCREEN_WIDTH;
    }
}

void updateBackground() {
    for (int i = 0; i < LAYER_COUNT; i++) {
        bgX1[i] -= layer_speed[i];
        bgX2[i] -= layer_speed[i];

        if (bgX1[i] <= -SCREEN_WIDTH) bgX1[i] = bgX2[i] + SCREEN_WIDTH;
        if (bgX2[i] <= -SCREEN_WIDTH) bgX2[i] = bgX1[i] + SCREEN_WIDTH;
    }
}

void renderBackground() {
    for (int i = 0; i < LAYER_COUNT; i++) {
        SDL_Rect bgRect1_src = {0, layer_cut[i], SCREEN_WIDTH, SCREEN_HEIGHT - layer_cut[i]};
        SDL_Rect bgRect2_src = {0, layer_cut[i], SCREEN_WIDTH, SCREEN_HEIGHT - layer_cut[i]};
        SDL_Rect bgRect1_dest = {bgX1[i], 0, SCREEN_WIDTH, SCREEN_HEIGHT - layer_cut[i]};
        SDL_Rect bgRect2_dest = {bgX2[i], 0, SCREEN_WIDTH, SCREEN_HEIGHT - layer_cut[i]};

        SDL_RenderCopy(renderer, layers[i], &bgRect1_src, &bgRect1_dest);
        SDL_RenderCopy(renderer, layers[i], &bgRect2_src, &bgRect2_dest);
    }
}

void closeBackgrounds() {
    for (int i = 0; i < LAYER_COUNT; i++) {
        SDL_DestroyTexture(layers[i]);
    }
}
