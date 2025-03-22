#include "score.h"
#include "background.h"

int score = 0;
int highScore = 0;
TTF_Font* font = TTF_OpenFont("font/pixel_font.ttf", 24);;
SDL_Color textColor = {50, 50, 50};
SDL_Texture* scoreTexture = nullptr;
SDL_Texture* highScoreTexture = nullptr;
SDL_Rect scoreRect = {800, 20, 100, 30};
SDL_Rect highScoreRect = {800, 50, 150, 30};

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

void renderscore(){
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreRect);
}

void closeScore(){
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(highScoreTexture);
    TTF_CloseFont(font);
    TTF_Quit();
}


