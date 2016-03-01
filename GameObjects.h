#ifndef GAMEOBJECTS_H_INCLUDED
#define GAMEOBJECTS_H_INCLUDED

#include <SFML/Graphics.hpp>

struct Bonus
{
    int x, y;
    double radius;

    Bonus() : x(0), y(0), radius(0) {}

    Bonus(int arg_x, int arg_y) : x(arg_x), y(arg_y), radius(5.0) {}
};

struct Bomb
{
    int x, y, width, height, delay;
    double radius;
    int timer;
    bool ready;

    Bomb(int arg_x, int arg_y) : x(arg_x), y(arg_y), radius(5.0)
    {
        reset();
    }

    void update()
    {
        timer++;
        ready = (timer == delay);
    }

    void reset()
    {
        ready = false;
        timer = 0;
        delay = rand()%200+10;
    }
};

#endif // GAMEOBJECTS_H_INCLUDED
