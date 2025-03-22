#include "character.h"
#include "background.h"

Character::Character() : texture(nullptr), frameIndex(0), animationDelay(0), isJumping(false), velocityY(0), gravity(0.5           ){}

void Character::init(SDL_Texture* tex, int x, int y, int w, int h) {
    texture = tex;
    for (int i = 0; i < 6; i++) {
        frames[i] = {i * w, 0, w, h};
    }
    destRect = {x, y, w*1.5, h*1.5};
}
void Character::Init()
{
    SDL_Texture* charTexture = LoadTexture("images/character/char.png",renderer);
    init(charTexture,100,GROUND_LEVEL,57,57);

}
void Character::update() {
    animationDelay++;
    if (animationDelay > 5) {
        frameIndex = (frameIndex + 1) % 6;
        animationDelay = 0;
    }

    if (isJumping) {
        destRect.y -= velocityY;
        velocityY -= gravity;
        if (destRect.y >= GROUND_LEVEL) {
            destRect.y = GROUND_LEVEL;
            isJumping = false;
        }
    }
}
void Character::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, &frames[frameIndex], &destRect);
}

void Character::jump() {
    if (!isJumping){
        isJumping = true;
        velocityY = 13;
    }
}
SDL_Rect Character::getRect() { return destRect; }

void Character::reset(){
    destRect.y=GROUND_LEVEL;    // trở về mặt đất
    isJumping = false;          // Đảm bảo nhân vật không còn ở trạng thái nhảy
    velocityY = 0;              // Đặt lại vận tốc nhảy để tránh trục trặc khi khởi động lại
    frameIndex = 0;             // Reset lại hoạt ảnh về frame đầu tiên
    animationDelay = 0;         // Đặt lại bộ đếm hoạt ảnh

}
