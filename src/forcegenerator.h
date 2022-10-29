#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H
#include "object.h"

class ForceGenerator {
    public:
        ForceGenerator();
        virtual ~ForceGenerator();

        virtual void update() = 0;

        virtual double calculateEnergy() {
            return 0;
        }

        double energy = 0;
};

class GravAttract: public ForceGenerator {
    public:
        GravAttract(Object*, Object*);
        ~GravAttract();

        void update();

        Object* A;
        Object* B;
};

class Gravity: public ForceGenerator {
    public:
        Gravity(Object*);
        ~Gravity();

        void update();

        Object* A;
};

class ConstRadiusFixedPivot: public ForceGenerator {
    public:
        ConstRadiusFixedPivot(Object*, Object*);
        ~ConstRadiusFixedPivot();

        void update();

        Object* A;
        Object* B;
};

class ConstRadius: public ForceGenerator {
    public:
        ConstRadius(Object* A, Object* pivot);
        ~ConstRadius();

        void update();

        Object* A;
        Object* B;
};

class Spring: public ForceGenerator {
    public:
        Spring(Object*, Object*, double, double);
        ~Spring();

        void update();
        double calculateEnergy();

        Object* A;
        Object* B;
        double k;
        double r0;

};

#endif