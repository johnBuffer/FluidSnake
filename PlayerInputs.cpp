#include "PlayerInputs.h"

PlayerInputs::PlayerInputs() :
    EventProvider()
{

}

Move PlayerInputs::getMove()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        return Move::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        return Move::RIGHT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        return Move::UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        return Move::DOWN;
    }

    return Move::NONE;
}
