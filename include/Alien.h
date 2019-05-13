#ifndef ALIEN_H
#define ALIEN_H


//Alien atributes
#define ALIEN_HP 50
#define ALIEN_SPEED 300  //Module of the speed
#define ALIEN_ROT_SPD_DG -MINION_ROT_SPD_DG/2


#include "Component.h"
#include "Gameobject.h"
#include "Sprite.h"
#include "Minion.h"
#include "InputManager.h"
#include "Camera.h"
#include "Vec2.h"
#include <queue>
#include <vector>



using namespace std;

class Alien : public Component
{
public:
    Alien(GameObject& associated, int nMinions);
    ~Alien();

    void Start();
    void Update(float dt);
    void Render();
    bool Is(string type);

private:

    class Action{

        public:
//        Action(Action&)= delete;
        enum ActionType {MOVE, SHOOT};
        Action(ActionType type, float x, float y);

        ActionType type;
        Vec2 pos;
    };

    int nMinions;
    Vec2 speed;
    int hp;
    queue<Action> taskQueue;
    vector<weak_ptr<GameObject>> minionArray;
};

#endif // ALIEN_H
