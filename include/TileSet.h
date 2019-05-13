#ifndef TILESET_H
#define TILESET_H

#include "Sprite.h"
#include "Gameobject.h"

using namespace std;

class TileSet
{
public:
    TileSet(int tileWidth, int tileHeight, string file, GameObject& associated);

    void RenderTile(unsigned index, float x, float y, int layer=0);
    int GetTileWidth();
    int GetTileHeight();

private:
    Sprite tileSet;

    int rows;
    int columns;

    int tileWidth;
    int tileHeight;
};

#endif // TILESET_H
