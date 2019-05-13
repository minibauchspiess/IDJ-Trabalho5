#include "include/Bullet.h"

Bullet::Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance, string sprite) :  Component(associated)
{
    //Add sprite component
    Sprite* bSprite = new Sprite(sprite, associated);
    associated.AddComponent(bSprite);

    //Initialize variables
    this->speed = Vec2(speed, 0);
    this->speed = this->speed.RotRad(angle);
    distanceLeft = maxDistance;
    this->damage = damage;

    //Set orientation of go
    associated.angleDeg = angle * (180/PI);
}

void Bullet::Update(float dt){
    Vec2 step = Vec2::Mul(speed, dt);               //Calculate the step
    distanceLeft = distanceLeft - Vec2::Mag(step);  //Atualize bullet lifetime
    if(distanceLeft>0){                             //Move it if it still has distance to move
        associated.box.x = associated.box.x + step.x;
        associated.box.y = associated.box.y + step.y;
    }
    else{
        associated.RequestDelete();
    }
}

void Bullet::Render(){}

bool Bullet::Is(string type){
    return (!type.compare("Bullet"));
}

int Bullet::GetDamage(){
    return damage;
}










