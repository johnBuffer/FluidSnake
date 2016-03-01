#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>

#include "Snake.h"
#include "Wave.h"
#include "GameObjects.h"
#include "GameSound.h"
#include "GameWorld.h"
#include "GameAction.h"
#include "PlayerInputs.h"
#include "ReplayInputs.h"

std::string intToString(int i)
{
    std::ostringstream convert;   // stream used for the conversion
    convert << i;      // insert the textual representation of 'Number' in the characters in the stream
    return convert.str();
}

std::vector<WavePoint>* Wave::surround = NULL;

int main(int argc, const char* argv[])
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(500, 500), "Snake", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    GameSound soundManager("sounds");

    Snake snake(25, 23);

    double life = snake.getLife();

    std::list<Bonus> deadBonus;
    std::list<Wave> waves;

    const std::string str_blur_h = "uniform sampler2D image;uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);void main(){vec2 TexCoords = gl_TexCoord[0].xy; vec2 tex_offset = 1.0 / textureSize(image, 0); vec3 result = texture(image, TexCoords).rgb * weight[0]; for(int i = 1; i < 5; ++i){result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];}gl_FragColor = vec4(result, 1.0);}";
    const std::string str_blur_v = "uniform sampler2D image;uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);void main(){vec2 TexCoords = gl_TexCoord[0].xy; vec2 tex_offset = 1.0 / textureSize(image, 0); vec3 result = texture(image, TexCoords).rgb * weight[0]; for(int i = 1; i < 5; ++i){result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];}gl_FragColor = vec4(result, 1.0);}";
    sf::Shader blur_h, blur_v;
    blur_h.loadFromMemory(str_blur_h, sf::Shader::Fragment);
    blur_v.loadFromMemory(str_blur_v, sf::Shader::Fragment);

    //soundManager.play("ambience");

    sf::RenderTexture screen, bloom, bloomRenderer;
    screen.create(500, 500);
    bloom.create(500, 500);
    bloomRenderer.create(375, 375);

    waves.push_front(Wave(25, 23, 30, 1));
    waves.front().ampl = 400;

    int score = 0;

    int choosingVx = 1;
    int choosingVy = 0;
    int vx = 1;
    int vy = 0;

    srand(time(NULL));

    std::list<Bomb> bombs;

    bool danger(false), waiting(false);
    int bombCount = 0;
    int dangerCount = 0;
    sf::Clock dangerClock;

    std::vector<WavePoint> map;
    for (int i(0); i<50; ++i)
    {
        for (int j(0); j<46; ++j) {map.push_back(WavePoint(i, j));}
    }

    Wave::surround = &map;

    int moveDelay = 75;
    double yScore = 0;
    bool newScore = false;

    sf::Text scoreText, scoreTransition, warning;
    sf::Font font;
    font.loadFromFile("font.ttf");
    scoreText.setFont(font);
    scoreText.setCharacterSize(38);
    scoreText.setString("0");

    scoreTransition = scoreText;
    warning = scoreText;
    warning.setString("WARNING <!>");
    warning.setCharacterSize(20);

    int heightScore = 40;

    bool gameOver = false;

    sf::Clock speedClock, bonusClock, timer;
    long tick = 0;

    std::vector<GameAction> replay;

    EventProvider* eventProvider;

    if (argc > 1)
        eventProvider = new ReplayInputs("replay");
    else
        eventProvider = new PlayerInputs();

    Bonus bonus;
    if (eventProvider->getBonusLocation(bonus.x, bonus.y))
    {
        bonus = Bonus(rand()%50, rand()%45);
    }

    replay.push_back(GameAction(tick, 1, bonus.x, bonus.y, 0, 0, 0));

    while (window.isOpen())
    {
        tick++;
        double time = timer.getElapsedTime().asSeconds();

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        Move snakeMove = eventProvider->getMove();

        if (snakeMove == Move::LEFT)
        {
            if (!vx)
            {
                choosingVx = -1;
                choosingVy = 0;
            }
        }
        else if (snakeMove == Move::RIGHT)
        {
            if (!vx)
            {
                choosingVx = 1;
                choosingVy = 0;
            }
        }
        else if (snakeMove == Move::UP)
        {
            if (!vy)
            {
                choosingVx = 0;
                choosingVy = -1;
            }
        }
        else if (snakeMove == Move::DOWN)
        {
            if (!vy)
            {
                choosingVx = 0;
                choosingVy = 1;
            }
        }

        if (bonusClock.getElapsedTime().asMilliseconds() >= 2000)
        {
            waves.push_front(Wave(bonus.x, bonus.y, 5, 1));
            waves.front().ampl = 50;

            soundManager.play("bip");
            bonusClock.restart();
        }

        if (bombCount == 10 && !waiting && !danger)
        {
            waiting = true;
            dangerClock.restart();
        }

        if (dangerClock.getElapsedTime().asSeconds() > 5 && !danger && waiting)
        {
            waiting = false;

            dangerCount++;
            bombCount += dangerCount;

            int simultaneous = dangerCount;
            if (simultaneous > 3) simultaneous = 3;

            for (int i(0); i<simultaneous; ++i)
            {
                Bomb b(0, 0);
                if (eventProvider->getBombLocation(b.x, b.y, b.delay))
                {
                    b = Bomb(rand()%50, rand()%45);
                }
                bombs.push_back(b);
                replay.push_back(GameAction(tick, 2, b.x, b.y, 0, 0, b.delay));
            }

            danger = true;
        }

        for (Bomb& bomb : bombs)
        {
            if (bomb.ready)
            {
                //bomb.reset();
                waves.push_back(Wave(bomb.x, bomb.y, 10, 10)); waves.back().ampl = 250;
                waves.push_back(Wave(bomb.x, bomb.y, 40, 1)); waves.back().ampl = 200;
                soundManager.play("boom");
                soundManager.play("bonus");

                const SnakeNode& head = snake.getHead();
                double vx = head.x-bomb.x;
                double vy = head.y-bomb.y;

                double distToSnake = sqrt(vx*vx+vy*vy);

                if (distToSnake < 15)
                    snake.addLife(-100/(distToSnake+1));

                /*
                bomb.x = rand()%50;
                bomb.y = rand()%46;
                */

                if (bombCount > 0)
                {
                    Bomb b(0, 0);
                    if (eventProvider->getBombLocation(b.x, b.y, b.delay))
                    {
                        b = Bomb(rand()%50, rand()%45);
                    }
                    bombs.push_back(b);
                    replay.push_back(GameAction(tick, 2, b.x, b.y, 0, 0, b.delay));
                    bombCount--;
                }
            }
        }

        bombs.remove_if([](Bomb& b){ return b.ready; });

        if (bombs.size() == 0)
            danger = false;

        if (tick%4==0 && !snake.isDead())
        {
            SnakeNode head = snake.getHead();

            if (vx != choosingVx || vy != choosingVy)
                replay.push_back(GameAction(tick, 0, 0, 0, choosingVx, choosingVy, 0));

            vx = choosingVx;
            vy = choosingVy;

            snake.setDirection(vx, vy);

            if (head.x+vx == bonus.x && head.y+vy == bonus.y)
            {
                if (!danger)
                    { bombCount++; }
                else
                    { score+= dangerCount-1; }

                //soundBonus.play();
                //soundBonusplus.play();

                soundManager.play("bonus");

                waves.push_front(Wave(bonus.x, bonus.y, 10, 4));

                snake.addNode(bonus.x, bonus.y);
                snake.addLife(5);

                deadBonus.push_front(bonus);
                if (eventProvider->getBonusLocation(bonus.x, bonus.y))
                {
                    bonus = Bonus(rand()%48+1, rand()%43+1);
                }

                replay.push_back(GameAction(tick, 1, bonus.x, bonus.y, 0, 0, 0));

                newScore = true;
                score++;
                scoreTransition.setString(intToString(score));
            }
            else
            {
                snake.progress();

                if (!(tick%6))
                {
                    waves.push_front(Wave(head.x, head.y, 11, 1));
                    waves.front().ampl = 20;
                }
            }
        }
        else if (snake.isDead())
        {
            if (!gameOver)
                soundManager.play("lose");

            for (const SnakeNode& sn : snake.getNodeList())
            {
                waves.push_front(Wave(sn.x, sn.y, 25, 2));
                waves.front().ampl = 20;
            }

            snake.kill();
            //ambience.setVolume(ambience.getVolume()*0.99);
            gameOver = true;
        }

        snake.update();

        for (Bomb& bomb : bombs)
            bomb.update();

        if (newScore)
        {
            if (heightScore-yScore < 0.1)
            {
                scoreText.setColor(sf::Color::White);
                newScore = false;
                yScore = 0;
                scoreText.setString(intToString(score));
            }
            else
            {
                yScore += (heightScore-yScore)/8.0;
                double alpha = 1-(yScore/double(heightScore));
                scoreText.setColor(sf::Color(255, 255, 255, 255*alpha));
                scoreTransition.setColor(sf::Color(255, 255, 255, 255*(1-alpha)));
            }
        }

        scoreText.setPosition(5, yScore-8);
        scoreTransition.setPosition(5, yScore-heightScore-8);

        // disparition des bonus morts
        for (Bonus& db : deadBonus) { if (db.radius < 50) db.radius += 2.5; }
        deadBonus.remove_if([](Bonus& b){ return b.radius >= 50; });

        for (WavePoint& wp : map)
        {
            wp.color = 25;
            wp.delta = 0;
        }

        for (Wave& wave : waves) { wave.apply(); }

        waves.remove_if([](Wave& v){ return v.done; });

        screen.clear(sf::Color::Black);
        bloom.clear(sf::Color::Black);

        int n_points = map.size();
        sf::VertexArray background(sf::Quads, n_points*4);
        for (int i=0; i<n_points; i++)
        {
            WavePoint& wp = map[i];
            sf::Color col = sf::Color(wp.color, wp.color, wp.color);
            double x = wp.x*10-wp.delta;
            double y = wp.y*10-wp.delta+heightScore;
            int g = 1;
            if (wp.delta > 4) g = wp.delta/4.0;

            background[i*4  ].position = sf::Vector2f(x-g, y-g);
            background[i*4+1].position = sf::Vector2f(x+g, y-g);
            background[i*4+2].position = sf::Vector2f(x+g, y+g);
            background[i*4+3].position = sf::Vector2f(x-g, y+g);

            background[i*4  ].color = col;
            background[i*4+1].color = col;
            background[i*4+2].color = col;
            background[i*4+3].color = col;
        }
        screen.draw(background);

        for (Bonus& db : deadBonus)
        {
            sf::CircleShape node(db.radius);
            node.setOrigin(db.radius, db.radius);
            double alpha = 1-(db.radius)/50.0;
            node.setFillColor(sf::Color(255, 255, 255, 255*alpha));
            node.setPosition(10*db.x+5, 10*db.y+5+heightScore);
            screen.draw(node);
            bloom.draw(node);
        }

        snake.draw(screen, bloom);

        sf::CircleShape node(5);
        node.setOrigin(5, 5);
        node.setFillColor(sf::Color(255, 255, 0));
        node.setPosition(10*bonus.x+5, 10*bonus.y+5+heightScore);

        screen.draw(node);

        node.setFillColor(sf::Color(255, 255, 100));
        double bonusScale = 0.8+0.7*pow(sin(3.14159*time/2), 2);
        node.scale(bonusScale, bonusScale);
        bloom.draw(node);

        for (Bomb& bomb : bombs)
        {
            sf::RectangleShape node(sf::Vector2f(10, 10));
            node.setOrigin(5, 5);
            node.setScale(1.2, 1.2);
            node.setFillColor(sf::Color(100, 0, 0));
            node.setPosition(bomb.x*10+5, bomb.y*10+5+heightScore);
            screen.draw(node);

            node.setFillColor(sf::Color(150, 0, 0));
            double bombScale = 1+0.8*pow(sin(3.14159*time*2.0), 2.0);
            node.scale(bombScale, bombScale);
            bloom.draw(node);
            node.setFillColor(sf::Color::White);
            node.scale(bombScale/4.0, bombScale/4.0);
            bloom.draw(node);
        }

        life += (snake.getLife()-life)/8.0;
        sf::RectangleShape lifeBar(sf::Vector2f(2*life, 10));
        lifeBar.setOrigin(life, 5);
        lifeBar.setFillColor(sf::Color(255-255*life/100.0, 255*life/100.0, 0));
        lifeBar.setPosition(500-life-10, 16);
        screen.draw(lifeBar);
        bloom.draw(lifeBar);

        sf::VertexArray lifeFrame(sf::LinesStrip, 5);
        lifeFrame[0].position = sf::Vector2f(289, 9);
        lifeFrame[1].position = sf::Vector2f(289+203, 9);
        lifeFrame[2].position = sf::Vector2f(289+203, 9+13);
        lifeFrame[3].position = sf::Vector2f(288, 9+13);
        lifeFrame[4].position = sf::Vector2f(289, 9);
        screen.draw(lifeFrame);

        if (danger)
        {
            warning.setPosition(105, 5);
            double c = 50*sin(time*20);
            warning.setColor(sf::Color(255, 100+c, 100+c));
            bloom.draw(warning);

            warning.setColor(sf::Color::Red);
            warning.setStyle(sf::Text::Bold);
            screen.draw(warning);
        }
        else if (waiting)
        {
            warning.setPosition(105, 5);
            double c = 50*sin(time*5);
            warning.setColor(sf::Color(50+c, 10, 10));
            bloom.draw(warning);

            warning.setColor(sf::Color(50+c, 0, 0));
            warning.setStyle(sf::Text::Bold);
            screen.draw(warning);
        }

        screen.display();
        sf::Sprite sprite(screen.getTexture());
        bloom.display();
        sf::Sprite spriteBloom(bloom.getTexture());
        spriteBloom.setScale(0.75, 0.75);
        bloomRenderer.draw(spriteBloom);
        sf::Sprite spriteBloomRenderer(bloomRenderer.getTexture());
        for (int i(0); i<2; ++i)
        {
            bloomRenderer.draw(spriteBloomRenderer, &blur_h);
            bloomRenderer.draw(spriteBloomRenderer, &blur_v);
        }
        spriteBloomRenderer.setScale(1.333, 1.333);

        bloomRenderer.display();

        window.clear(sf::Color::Black);

        window.draw(sprite);
        window.draw(spriteBloomRenderer, sf::BlendAdd);
        window.draw(scoreText);
        window.draw(scoreTransition);

        window.display();
    }

    std::ofstream file("replay");
    file << replay.size() << "\n";
    for (GameAction& ga : replay)
    {
        file << ga.tick << " " << ga.type << " " << ga.x << " " << ga.y << " " << ga.vx << " " << ga.vy << " " << ga.timer << "\n";
    }
    file.close();

    return 0;
}
