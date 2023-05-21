#ifndef OBJECT_H
#define OBJECT_H

#include "vector2d.h"
#include <vector>
#include <SDL2/SDL.h>

class Object {
    public:
        Object() {}
        virtual ~Object() {}

        virtual void applyForce(Vector2d F_);
        virtual void applyForce(double xmag, double ymag);
        virtual void applyForceAtPoint(Vector2d F_, Vector2d R_);
        void setColor(int, int, int, int);
        virtual void render(SDL_Renderer*);
        virtual void renderCircle(SDL_Renderer*);
        double calculateEnergy();

        Vector2d acc;
        Vector2d vel;
        Vector2d pos;
        double I;
        double theta = 0;
        double omega = 0;
        double alpha = 0;
        Vector2d F;
        double mass = 0;
        double energy = 0;

        int R = 255;
        int G = 255;
        int B = 255;
        int A = 255;

        int index = -1;
};

class ImmovableObject: public Object {
    public:
        ImmovableObject() {}
        ~ImmovableObject() {}

        void applyForce(Vector2d F_) {}
        void render(SDL_Renderer* rend) {
            Object::render(rend);
        }

};

#endif