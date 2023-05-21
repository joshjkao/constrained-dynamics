#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include "object.h"
#include <SDL2/SDL.h>

class Constraint {
    public:
        Constraint() {}
        virtual ~Constraint() {}

        virtual void addJacobian(std::vector<double>&, std::vector<double>&) = 0;
        virtual void render(SDL_Renderer* rend) = 0;
};

class DistanceConstraintFixed: public Constraint {
    public:
        DistanceConstraintFixed(Object*, Vector2d);
        ~DistanceConstraintFixed();

        void addJacobian(std::vector<double>&, std::vector<double>&);
        void render(SDL_Renderer* rend);        

    private:
        Object* obj;
        Vector2d pivot;
};

class DistanceConstraint: public Constraint {
    public:
        DistanceConstraint(Object*, Object*);
        ~DistanceConstraint();

        void addJacobian(std::vector<double>&, std::vector<double>&);
        void render(SDL_Renderer* rend);

    private:
        Object* obj1;
        Object* obj2;
};

#endif