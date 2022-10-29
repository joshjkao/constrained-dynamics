#ifndef DOUBLEPENDULUM_H
#define DOUBLEPENDULUM_H

#include "vector2d.h"
#include <SDL2/SDL.h>

class LagrangianDoublePendulum {
    public:
        LagrangianDoublePendulum();
        ~LagrangianDoublePendulum();

        void update();
        void render(SDL_Renderer*);

        double theta1 = -M_PI_2;
        double theta2 = -M_PI_2;
        double theta1dot = 0;
        double theta2dot = 0;
        double theta1ddot = 0;
        double theta2ddot = 0;

        Vector2d pivot;
        Vector2d string1;
        Vector2d string2;

        double r1=1;
        double r2=1;
        double m1=0.5;
        double m2=0.5;

        double dt = .0001/5;
        double g = 9.8;

    private:
        void updateVectors();
};

#endif