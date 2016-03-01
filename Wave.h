#ifndef WAVE_H_INCLUDED
#define WAVE_H_INCLUDED

#include <vector>
#include <SFML/Graphics.hpp>

struct WavePoint
{
    int x, y;
    double delta, color;

    WavePoint() : delta(0), color(25) {}
    WavePoint(int arg_x, int arg_y) : x(arg_x), y(arg_y), delta(0), color(25) {}
};

struct Wave
{
    int x, y;
    double ampl, speed, phi, t, totalInfluence;
    static std::vector<WavePoint> *surround;
    bool done;

    Wave(int arg_x, int arg_y, double arg_speed, double arg_phi) :
        x(arg_x),
        y(arg_y),
        ampl(200),
        speed(arg_speed),
        phi(arg_phi),
        t(0),
        done(false) {}

    void apply()
    {
        totalInfluence = 0;
        t += 0.016;

        for (WavePoint& wp : *surround)
        {
            double vx = wp.x - x - 1;
            double vy = wp.y - y - 1;

            double dist = sqrt(vx*vx+vy*vy);
            double delta = -dist/phi;
            double amp = ampl/(dist+10);

            if (speed*t+delta < 3.14159 && speed*t+delta > 0)
            {
                double move = sin(speed*t+delta);
                totalInfluence += move;
                double reflect = (10*amp)*move*move;

                wp.delta += amp*move;
                wp.color += reflect;

                if (wp.color > 255) wp.color = 255;
            }
        }

        done = (totalInfluence == 0);
    }
};

#endif // WAVE_H_INCLUDED
