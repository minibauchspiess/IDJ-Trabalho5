
#include "include/Sprite.h"
#include "include/Game.h"
#include "include/Resources.h"
#include <iostream>

using namespace std;

Sprite::Sprite(GameObject& associated) : Component(associated)
{
    texture = nullptr;
    scale = Vec2(1, 1);
    angleDeg = associated.angleDeg;
}

Sprite::Sprite(string file, GameObject& associated) : Component(associated){
    texture =nullptr;
    Open(file);
    scale = Vec2(1, 1);
    angleDeg = associated.angleDeg;
}

Sprite::~Sprite(){
}

void Sprite::Update(float dt){}

bool Sprite::Is(string type){
    return (!type.compare("sprite"));
}

void Sprite::Open(string file){

    int width, height;

    texture = Resources::GetImage(file);

    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    associated.box.w = width;
    associated.box.h = height;
    SetClip(0, 0, width, height);

}

void Sprite::SetClip(int x, int y, int w, int h){
    clipRect.x=x;
    clipRect.y=y;
    clipRect.w=w;
    clipRect.h=h;
}

void Sprite::Render(){
    Render(associated.box.x, associated.box.y, associated.box.w, associated.box.h);
}

void Sprite::Render(int x, int y, int w, int h, int layer){
    //cout<<associated.layer<<endl;
    Game& instance = Game::GetInstance();

    SDL_Rect dstrect;
    dstrect.x = x - (int)(Camera::pos.x*(layer+1)*LAYER_MOV);
    dstrect.y =y - (int)(Camera::pos.y*(layer+1)*LAYER_MOV);

    dstrect.w = w;
    dstrect.h = h;

    SDL_RenderCopyEx(instance.GetRenderer(), texture, &clipRect, &dstrect, associated.angleDeg, nullptr, SDL_FLIP_NONE);
}

int Sprite::GetWidth(){
    return associated.box.w * scale.x;
}

int Sprite::GetHeight(){
    return associated.box.h * scale.y;
}

bool Sprite::IsOpen(){
    if (texture != nullptr){
        return true;
    }
    else{
        return false;
    }
}

void Sprite::SetScale(float scaleX, float scaleY){
    if(scaleX){     //Any value different than 0 changes the scale
        scale.x = scaleX;
    }
    if(scaleY){
        scale.y = scaleY;
    }

    //Adjust object box with scale
    associated.box.x = associated.box.x - ((associated.box.w * scale.x) - associated.box.w)/2;
    associated.box.y = associated.box.y - ((associated.box.h * scale.y) - associated.box.h)/2;

    associated.box.w = associated.box.w * scale.x;
    associated.box.h = associated.box.h * scale.y;
}

Vec2 Sprite::GetScale(){
    return scale;
}


