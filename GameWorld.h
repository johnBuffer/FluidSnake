#ifndef GAMEWORLD_H_INCLUDED
#define GAMEWORLD_H_INCLUDED

#include "Snake.h"
#include "Wave.h"
#include "GameObjects.h"

class GameWorld
{
public:
    GameWorld(int width, int height);

    void update();

private:
    int _width, _height;

    Snake _snake;
    Bonus _bonus;

    std::list<Wave> _waves;
    std::vector<WavePoint> _background;

    std::list<Bomb> _bombs;
};

#endif // GAMEWORLD_H_INCLUDED
