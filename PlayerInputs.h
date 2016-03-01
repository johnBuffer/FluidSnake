#ifndef PLAYERINPUTS_H_INCLUDED
#define PLAYERINPUTS_H_INCLUDED

#include "EventProvider.h"

class PlayerInputs : public EventProvider
{
public:
    PlayerInputs();

    Move getMove();
    bool getBombLocation(int& x, int& y, int& timer) {return true;}
    bool getBonusLocation(int& x, int &y) {return true;}
};


#endif // PLAYERINPUTS_H_INCLUDED
