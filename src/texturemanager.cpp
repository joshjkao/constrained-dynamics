#include "texturemanager.h"
#include <iostream>

SDL_Texture* TextureManager::loadTexture(const char* filename) {
    SDL_Surface* temp = IMG_Load(filename);
    std::cout << "loaded image" << std::endl;
    SDL_Texture* output = SDL_CreateTextureFromSurface(App::renderer, temp);
    std::cout << "created texture" << std::endl;
    SDL_FreeSurface(temp);

    return output;
}

void TextureManager::render(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, double theta) {
    SDL_Point pivot;
    pivot.y = 0;
    pivot.x = dest.w/2;
    SDL_RenderCopyEx(App::renderer, texture, &src, &dest, theta, &pivot, SDL_FLIP_NONE);
}  