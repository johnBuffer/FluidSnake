#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include <list>
#include <SFML/Graphics.hpp>
#include <cmath>

struct SnakeNode
{
    int x, y;
    double c, maxC;
    SnakeNode(int arg_x, int arg_y, double arg_maxC=10) : x(arg_x), y(arg_y), maxC(arg_maxC) {}
};

class Snake
{
public:
    Snake(int arg_x, int arg_y);

    void update();
    void progress();
    void kill() {_snake.clear();}
    void draw(sf::RenderTarget& renderer, sf::RenderTarget& rendererBloom);
    void addNode(int x, int y);
    void setDirection(int vx, int vy) {_vx = vx; _vy = vy;}
    void addLife(double life);

    const SnakeNode& getHead() const;
    const std::list<SnakeNode>& getNodeList() const {return _snake;}
    bool isDead() const {return _dead;}
    double getLife() const {return _life;}

private:
    void checkAutoCollision();

    std::list<SnakeNode> _snake, _deadNodes;
    int _vx, _vy;
    double _life;
    bool _dead;

    sf::Clock _timer;
};

#endif // SNAKE_H_INCLUDED
