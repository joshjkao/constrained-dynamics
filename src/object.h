#ifndef OBJECT_H
#define OBJECT_H

#include "vector2d.h"
#include <vector>
#include <SDL2/SDL.h>

class Object {
    public:
        Object() {}
        virtual ~Object() {}

        virtual void applyForce(Vector2d);
        virtual void render(SDL_Renderer*);
        double calculateEnergy();

        Vector2d acc;
        Vector2d vel;
        Vector2d pos;
        Vector2d F;
        double mass = 0;
        double energy = 0;

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