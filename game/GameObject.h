#pragma once 

#include "Game.h"


class GameObject {
    public :
        GameObject(const char* texsheet, SDL_Renderer* ren, int x, int y);
        ~GameObject();

        void gUpdate();
        void gRender();

    private :
        int xpos;
        int ypos;

        SDL_Texture* objTexture;
        SDL_Rect srcRect, destRect;
        SDL_Renderer* renderer;

};