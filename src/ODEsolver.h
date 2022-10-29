#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H
#include "system.h"

class ODESolver {
    public:
        ODESolver() {}
        virtual ~ODESolver() {}

        virtual void start(System*, double) = 0;
        virtual void iterate(System*) = 0;

    protected:
        double dt;
};

class EulerSolver: public ODESolver {
    public:
        EulerSolver();
        ~EulerSolver();
        void start(System*, double);
        void iterate(System*);
};

#endif