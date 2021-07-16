#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexturefromFile(const char* texture , SDL_Renderer* trenderer )
{
    SDL_Surface* tempSurface = IMG_Load(texture);
    SDL_Texture* temp_tex = SDL_CreateTextureFromSurface(trenderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return temp_tex;
}