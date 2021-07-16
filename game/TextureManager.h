#pragma once

#include "Game.h"

class TextureManager {

    public: 
        static SDL_Texture* LoadTexturefromFile(const char* filename , SDL_Renderer* trenderer );

};
