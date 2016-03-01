#ifndef EVENTPROVIDER_H_INCLUDED
#define EVENTPROVIDER_H_INCLUDED

#include <SFML/Graphics.hpp>

enum Move
{
    NONE = -1,
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

class EventProvider
{
public:
    EventProvider();

    virtual Move getMove() = 0;
    virtual bool getBombLocation(int& x, int& y, int& timer) = 0;
    virtual bool getBonusLocation(int& x, int &y) = 0;

private:

};

#endif // EVENTPROVIDER_H_INCLUDED
