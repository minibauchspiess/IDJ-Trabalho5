#include "include/State.h"
#include "dirent.h"
#include "include/Vec2.h"
#include "include/Rect.h"
#include "include/Face.h"
#include "include/Sound.h"

#define PI 3.14159265358979323846

using namespace std;



State::State()
{

    started = false;

    //Background object
    GameObject* bgObj = new GameObject();
    Sprite* bg = new Sprite("assets/img/ocean.jpg", *bgObj);
    bgObj->AddComponent(bg);
    CameraFollower* bgCamera = new CameraFollower(*bgObj);
    bgObj->AddComponent(bgCamera);
    //objectArray.emplace_back(bgObj);
    AddObject(bgObj);


    //TileMap object
    GameObject* tmObj = new GameObject(-1);                     //Layer -1 tels state it is the TileMap object
    TileSet* ts = new TileSet(64, 64, "assets/img/tileset.png", *tmObj);
    TileMap* tm = new TileMap(*tmObj, "assets/map/tileMap.txt", ts);
    tmObj->AddComponent(tm);
    tmObj->box.x = 0;
    tmObj->box.y = 0;
    //objectArray.emplace_back(tmObj);
    AddObject(tmObj);

    //Alien
    GameObject* alObj = new GameObject();
    Alien* alien = new Alien(*alObj, 3);
    alObj->AddComponent(alien);
    alObj->box.x = 512;
    alObj->box.y = 300;
    //objectArray.emplace_back(alObj);
    AddObject(alObj);

    quitRequested = false;

    music.Open("assets/audio/stageState.ogg");
    music.Play();
}

State::~State(){
    objectArray.clear();
}

void State::Start(){
    LoadAssets();

    //Calls Start of every object already in the vector
    for(unsigned int i=0;i<objectArray.size();i++){
        objectArray[i]->Start();
    }

    started = true;
}

void State::LoadAssets(){
    //Load things the game need
}

void State::Update(float dt){
    InputManager& input = InputManager::GetInstance();

    //Update camera position
    Camera::Update(dt);

    //Check user request to end program
    if(input.QuitRequested() || input.KeyPress(ESC)){
        quitRequested = true;
    }

    //If space was pressed, create face object
    if(input.KeyPress(SPACE)){
        Vec2 v1 = Vec2( 200, 0 ).RotRad( -PI + PI*(rand() % 1001)/500.0 );
        Vec2 v2 = Vec2( input.GetMouseX(), input.GetMouseY() );
        Vec2 objPos =  Vec2::Add(v1, v2);
        //AddObject((int)objPos.x, (int)objPos.y);
    }

    //Input();
    for (int i = 0; i < objectArray.size();){
        objectArray[i]->Update(dt);

        if (objectArray[i]->IsDead()){
            objectArray.erase(objectArray.begin()+i);
        }
        else{
            i++;
        }
    }
}

void State::Render(){
    for (int layer=0; layer<N_LAYERS;layer++) {         //Render objects with layer priority
        for (unsigned int i = 0; i < objectArray.size(); i++){
            if(objectArray[i]->layer == -1){            //TileMap, a multilayer GameObject
                objectArray[i]->layer = layer;
                objectArray[i]->Render();
            }
            else{
                if(layer == objectArray[i]->layer){
                    objectArray[i]->Render();
                }
            }

        }
    }

}

bool State::QuitRequested(){
    return quitRequested;
}


weak_ptr<GameObject> State::AddObject(GameObject* go){
    //Create shared pointer with given pointer
    shared_ptr<GameObject> shgo(go);

    //Insert it in the vector of game objects
    objectArray.push_back(shgo);

    //Start the new object
    if(started) shgo->Start();

    //Return it as a weak pointer
    weak_ptr<GameObject> wgo(shgo);
    return wgo;



    /*GameObject* enemy1 = new GameObject(0);

    Sprite *pg_face = new Sprite("assets/img/penguinface.png", *enemy1);
    enemy1->AddComponent((shared_ptr<Sprite>)pg_face);
    enemy1->box.x = mouseX - enemy1->box.w/2 + Camera::pos.x;
    enemy1->box.y = mouseY - enemy1->box.h/2 + Camera::pos.y;

    Sound *boomSnd = new Sound(*enemy1, "assets/audio/boom.wav");
    //boomSnd->Play(1);
    enemy1->AddComponent((shared_ptr<Sound>)boomSnd);
    Sound* som = (Sound*)enemy1->GetComponent("Sound").get();
    //som->Play(1);

    Face *face = new Face(*enemy1);
    enemy1->AddComponent((shared_ptr<Face>)face);

    objectArray.emplace_back(enemy1);*/
}


weak_ptr<GameObject> State::GetObjectPtr(GameObject *go){

    weak_ptr<GameObject> wgo;

    //Look for the game object in the vector of game objects
    for(unsigned i=0; i<objectArray.size(); i++) {
        if(objectArray[i].get() == go){
            wgo = objectArray[i];
            return wgo;
        }
    }

    return wgo;
}










