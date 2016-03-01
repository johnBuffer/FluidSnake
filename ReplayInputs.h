#ifndef REPLAYINPUTS_H_INCLUDED
#define REPLAYINPUTS_H_INCLUDED

#include <fstream>
#include <list>

#include "EventProvider.h"
#include "GameAction.h"

class ReplayInputs : public EventProvider
{
public:
    ReplayInputs(std::string replayFile);

    Move getMove();
    bool getBombLocation(int& x, int &y, int& timer);
    bool getBonusLocation(int& x, int &y);

private:
    int _tick;
    std::list<GameAction> _moves;
    std::list<GameAction> _bombLocation;
    std::list<GameAction> _bonusLocation;

    bool getNext(std::list<GameAction>&, GameAction&);
};

#endif // REPLAYINPUTS_H_INCLUDED
