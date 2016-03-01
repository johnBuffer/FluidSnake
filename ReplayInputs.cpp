#include "ReplayInputs.h"

#include <iostream>

ReplayInputs::ReplayInputs(std::string replayFile)
{
    _tick = 0;

    std::ifstream file(replayFile);

    if (!file)
        std::cout << "Error : cannot open file." << std::endl;
    else
        std::cout << "Loading replay..." << std::endl;

    int n;
    file >> n;
    std::cout << n << " ticks to load." << std::endl;
    for (int i(0); i<n; i++)
    {
        int type, tick, x, y, vx, vy, timer;
        file >> tick >> type >> x >> y >> vx >> vy >> timer;
        GameAction ga(tick, type, x, y, vx, vy, timer);
        if (type == 0)
            _moves.push_back(ga);
        else if (type == 1)
            _bonusLocation.push_back(ga);
        else
            _bombLocation.push_back(ga);
    }

    file.close();
    std::cout << "Done." << std::endl;
}

Move ReplayInputs::getMove()
{
    _tick++;
    if (_moves.front().tick == _tick)
    {
        GameAction ga = _moves.front();
        _moves.pop_front();

        if (ga.vx == -1)
            return Move::LEFT;
        else if (ga.vx == 1)
            return Move::RIGHT;
        else if (ga.vy == -1)
            return Move::UP;
        else if (ga.vy == 1)
            return Move::DOWN;
    }

    return Move::NONE;
}

bool ReplayInputs::getNext(std::list<GameAction>& l, GameAction& ga)
{
    if (l.size())
    {
        ga = (l.front());
        l.pop_front();

        return true;
    }

    return false;
}

bool ReplayInputs::getBombLocation(int& x, int& y, int& timer)
{
    GameAction bomb;

    if (getNext(_bombLocation, bomb))
    {
        x = bomb.x;
        y = bomb.y;
        timer = bomb.timer;

        return false;
    }

    return true;
}

bool ReplayInputs::getBonusLocation(int& x, int &y)
{
    GameAction bonus;

    if (getNext(_bonusLocation, bonus))
    {
        x = bonus.x;
        y = bonus.y;
        return false;
    }

    return true;
}



