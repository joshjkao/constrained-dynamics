#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include "object.h"

class Constraint {
    public:
        Constraint() {}
        virtual ~Constraint() {}

        virtual void addJacobian(std::vector<double>&, std::vector<double>&) = 0;
};

class DistanceConstraintFixed: public Constraint {
    public:
        DistanceConstraintFixed(Object*, Vector2d);
        ~DistanceConstraintFixed();

        void addJacobian(std::vector<double>&, std::vector<double>&);

    private:
        Object* obj;
        Vector2d pivot;
};

class DistanceConstraint: public Constraint {
    public:
        DistanceConstraint(Object*, Object*);
        ~DistanceConstraint();

        void addJacobian(std::vector<double>&, std::vector<double>&);

    private:
        Object* obj1;
        Object* obj2;
};

#endif