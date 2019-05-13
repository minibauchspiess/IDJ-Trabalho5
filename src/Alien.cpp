#include "include/Alien.h"
#include "include/Game.h"

Alien::Alien(GameObject& associated, int nMinions) : Component (associated)
{
    //Add the alien image to the object
    Sprite* alienFace = new Sprite("assets/img/alien.png", associated);
    associated.AddComponent(alienFace);

    //Variable initializing
    hp = ALIEN_HP;
    speed = Vec2(0,0);
    this->nMinions = nMinions;
}

Alien::Action::Action(Action::ActionType type, float x, float y) : type(type), pos(x, y){}

Alien::~Alien(){}

void Alien::Start(){
    //Pointer to the minion
    weak_ptr<GameObject> wmin;
    shared_ptr<GameObject> shmin;


    for (int i=0; i<nMinions; i++) {
        GameObject *minObj = new GameObject();                      //Create minion object
        wmin = Game::GetInstance().GetState().AddObject(minObj);    //Add it to the GO vector in state
        minionArray.emplace_back(wmin);                             //Add it to the minions vector

        float angOffset = (2*PI/this->nMinions)*i;
        Minion *minion = new Minion(*minObj, Game::GetInstance().GetState().GetObjectPtr(&associated), angOffset);

        angOffset *= (180/PI);  //Transform from rad to degrees
        minObj->angleDeg = angOffset - 90;  //If offset is 90º, minion will stay horizontal

        Sprite *minionSprite = (Sprite*) minObj->GetComponent("sprite").get();
        float scale = (rand() % 5) * 0.1 + 1;         //Random number between 1 and 1.5
        minionSprite->SetScale(scale, scale);
        minObj->AddComponent(minion);
    }
}

void Alien::Update(float dt){
    //Get input object to interact with user
    InputManager& input = InputManager::GetInstance();

    //Rotate with the angular speed given
    this->associated.angleDeg += ALIEN_ROT_SPD_DG*dt;

    //Left mouse button click makes alien shoot
    if(input.MousePress(LMOUSE)){
        Action actShoot(Action::SHOOT,
                        input.GetMouseX()  + Camera::pos.x,
                        input.GetMouseY()  + Camera::pos.y);

        taskQueue.emplace(actShoot);
    }
    //Right mouse button click makes alien move
    if(input.MousePress(RMOUSE)){
        Action actMove(Action::MOVE,
                       input.GetMouseX() + Camera::pos.x - associated.box.w/2,
                       input.GetMouseY() + Camera::pos.y - associated.box.h/2);

        taskQueue.emplace(actMove);
    }

    //Execute actions in queue
    if(!taskQueue.empty()){

        Vec2 step;      //Step vector to be added to the current position, in order to arrive at destination at constant velocity
        Action task = taskQueue.front();
        switch (task.type) {
            case(Action::MOVE):

                step = Vec2(task.pos.x - associated.box.x, task.pos.y - associated.box.y);     //Get step distance (in vector)

                //If Alien already in position (in the first click), dismiss command
                if((step.x == 0) && (step.y == 0)){
                    taskQueue.pop();
                }
                else{
                    step = Vec2::Mul(step, 1 / Vec2::Mag(step));     //Get unit vector of the direction
                    step = Vec2::Mul( step, ALIEN_SPEED * dt);          //Set step accordingly to direction, speed and passed time

                    //Check if Alien arrives in next frame at destination
                    if( (abs(task.pos.x - associated.box.x) <= abs(step.x)) && (abs(task.pos.y - associated.box.y) <= abs(step.y)) ){
                        associated.box.x = task.pos.x;
                        associated.box.y = task.pos.y;
                        taskQueue.pop();        //Remove task after its completion
                    }
                    //Move the given step, if still away enough from it
                    else{
                        associated.box.x = associated.box.x + step.x;
                        associated.box.y = associated.box.y + step.y;
                    }
                }
                break;


            case(Action::SHOOT):
                //Locate the closest minion
                float minDist;
                unsigned int minInd=0;
                if(minionArray.size()>0){           //Check if Alien has any minion to shoot
                    shared_ptr<GameObject> shMinion = minionArray[0].lock();   //Take minion object from vector
                    minDist = Vec2::Dist(shMinion->box.Center(), task.pos);

                    for (unsigned int i=1;i<minionArray.size();i++) {
                        shared_ptr<GameObject> shMinion = minionArray[i].lock();   //Take minion object from vector

                        float dist = Vec2::Dist(shMinion->box.Center(), task.pos);  //Find distance of current minion to the given point
                        if( dist  <  minDist ){     //New closest minion found
                            minDist = dist;
                            minInd = i;
                        }
                    }

                    //Make the closest minion found shoot in the given target
                    Minion *closeMinion = (Minion*) minionArray[minInd].lock()->GetComponent("Minion").get();   //Create pointer to the minion
                    closeMinion->Shoot(task.pos);




                }
                taskQueue.pop();
                break;

        }


    }

}

void Alien::Render(){}

bool Alien::Is(string type){
    return (!type.compare("Alien"));
}









