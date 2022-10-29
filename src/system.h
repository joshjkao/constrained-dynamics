#ifndef SYSTEM_H
#define SYSTEM_H
#include "object.h"
#include "forcegenerator.h"
#include "constraint.h"
#include "matrix.h"
#include <iostream>

class System {
    public:
        System();
        ~System();

        void addObject(Object*);
        void addForceGenerator(ForceGenerator*);
        void addConstraint(Constraint*);

        void clearForces();
        void applyForce(Vector2d F_, Object* object);
        void render(SDL_Renderer*);
        double calculateEnergy();
        void updateState();
        void calculateJacobian();
        void applyConstraintForces();

        void cleanup();

        void dump(std::ostream& os);

        std::vector<Object*> objects;
        std::vector<ForceGenerator*> generators;
        std::vector<Constraint*> constraints;

        std::vector<double> Q;
        std::vector<double> Qdot;
        std::vector<double> Fext;
        std::vector<double> Qddot;

        Matrix Minv;
        Matrix J;
        Matrix Jdot;
        Matrix JT;

        int n = 0;

        static double g;
        double energy = 0;
};

#endif