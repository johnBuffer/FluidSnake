#include "Snake.h"

#include <iostream>

Snake::Snake(int arg_x, int arg_y)
{
    addNode(arg_x, arg_y);
    _dead = false;
    _life = 100.0;
}

void Snake::addLife(double life)
{
    _life += life;
     if (_life < 0)
        _life = 0;
     else if (_life > 100)
        _life = 100;
}

void Snake::update()
{
    if (_life <= 0) _dead = true;

    // apparition de la tête
    for (SnakeNode& sn : _snake) { if (sn.c < sn.maxC) sn.c += 0.5; }

    // disparition des nodes morts
    for (SnakeNode& sn : _deadNodes) { if (sn.c > 0) sn.c -= 0.5; }
    _deadNodes.remove_if([](SnakeNode& sn){ return sn.c <= 0; });

}

void Snake::progress()
{
    SnakeNode head = _snake.front();
    _deadNodes.push_front(_snake.back());

    head.x += _vx;
    head.y += _vy;
    head.x %= 50;
    head.y %= 46;

    if (head.x < 0) head.x += 50;
    if (head.y < 0) head.y += 46;

    checkAutoCollision();

    _snake.push_front(head);
    _snake.pop_back();
}

void Snake::addNode(int x, int y)
{
    SnakeNode newNode = SnakeNode(x, y, 10);
    newNode.c = 10;
    _snake.push_front(newNode);
}

void Snake::checkAutoCollision()
{
    SnakeNode& head = _snake.front();
    int n_collision = 0;
    for (SnakeNode& sn : _snake)
    {
        if (head.x == sn.x && head.y == sn.y) { n_collision++; }
    }

    if (n_collision > 1)
    {
        _dead = true;
        _life = 0;
    }
}

void Snake::draw(sf::RenderTarget& renderer, sf::RenderTarget& rendererBloom)
{
    double time = _timer.getElapsedTime().asSeconds();
    double heightScore = 40;
    double lifeRatio = 1.0-_life/100.0;

    sf::VertexArray nodes(sf::Quads, _snake.size()*4);
    int i=0;
    bool endLight = true;
    for (const SnakeNode& sn : _snake)
    {
        double phi = time*lifeRatio*20-i/10.0;
        double c = 0;
        if (phi >= 0 && phi < 3.14159)
        {
            c = 150*sin(phi);
            endLight = false;
        }

        double r = lifeRatio*255+c; if (r>255) r = 255;
        double g = 255-lifeRatio*255+c; if (g>255) g = 255;
        double b = c; if (b>255) b = 255;
        sf::Color color(r, g, b);

        double x = sn.x*10+5;
        double y = sn.y*10+heightScore+5;
        double s = sn.c/2.0;

        nodes[i*4  ].position = sf::Vector2f(x-s, y-s);
        nodes[i*4+1].position = sf::Vector2f(x+s, y-s);
        nodes[i*4+2].position = sf::Vector2f(x+s, y+s);
        nodes[i*4+3].position = sf::Vector2f(x-s, y+s);

        nodes[i*4  ].color = color;
        nodes[i*4+1].color = color;
        nodes[i*4+2].color = color;
        nodes[i*4+3].color = color;

        i++;
    }

    renderer.draw(nodes);
    rendererBloom.draw(nodes);

    sf::VertexArray deadNodes(sf::Quads, _deadNodes.size()*4);
    int j = i;
    i=0;
    for (const SnakeNode& sn : _deadNodes)
    {
        double phi = time*lifeRatio*20-j/10.0;
        double c = 0;
        if (phi >= 0 && phi < 3.14159)
        {
            c = 255*sin(phi);
            endLight = false;
        }

        double r = lifeRatio*255+c; if (r>255) r = 255;
        double g = 255-lifeRatio*255+c; if (g>255) g = 255;
        double b = c; if (b>255) b = 255;
        sf::Color color(r, g, b);

        double x = sn.x*10+5;
        double y = sn.y*10+heightScore+5;
        double s = sn.c/2.0;

        deadNodes[i*4  ].position = sf::Vector2f(x-s, y-s);
        deadNodes[i*4+1].position = sf::Vector2f(x+s, y-s);
        deadNodes[i*4+2].position = sf::Vector2f(x+s, y+s);
        deadNodes[i*4+3].position = sf::Vector2f(x-s, y+s);

        deadNodes[i*4  ].color = color;
        deadNodes[i*4+1].color = color;
        deadNodes[i*4+2].color = color;
        deadNodes[i*4+3].color = color;

        i++;
    }

    if (endLight)
        _timer.restart();

    renderer.draw(deadNodes);
    rendererBloom.draw(deadNodes);
}

const SnakeNode& Snake::getHead() const
{
    return _snake.front();
}
