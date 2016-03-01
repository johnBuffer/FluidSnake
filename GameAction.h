#ifndef GAMEACTION_H_INCLUDED
#define GAMEACTION_H_INCLUDED

struct GameAction
{
    int tick, type, x, y, vx, vy, timer;

    GameAction() :
        tick(0),
        type(0),
        x(0),
        y(0),
        vx(0),
        vy(0),
        timer(0) {}

    GameAction(int arg_tick, int arg_type, int arg_x, int arg_y, int arg_vx, int arg_vy, int arg_timer) :
        tick(arg_tick),
        type(arg_type),
        x(arg_x),
        y(arg_y),
        vx(arg_vx),
        vy(arg_vy),
        timer(arg_timer) {}
};

#endif // GAMEACTION_H_INCLUDED
