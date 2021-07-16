#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texsheet, SDL_Renderer* ren, int x , int y)
{
    renderer = ren;
    objTexture = TextureManager::LoadTexturefromFile(texsheet,ren);

    xpos = x;
    ypos = y;

}

void GameObject::gUpdate()
{
    xpos ++;
    ypos ++;

    srcRect.h = 1400;
    srcRect.w = 1200;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = 64;
    destRect.h = 64;
    
}

void GameObject::gRender()
{
    SDL_RenderCopy(renderer,objTexture,&srcRect,&destRect);
}