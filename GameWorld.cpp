#include "GameWorld.h"

GameWorld::GameWorld(int width, int height) :
    _width(width),
    _height(height),
    _snake(25, 23),
    _bonus(0, 0)
{
    _waves.push_front(Wave(25, 23, 30, 1));
    _waves.front().ampl = 400;
}
