#include "ODEsolver.h"

// Euler Implementation
EulerSolver::EulerSolver() {

}

EulerSolver::~EulerSolver() {
    
}

void EulerSolver::start(System* system, double dt_) {
    dt = dt_;
}

void EulerSolver::iterate(System* system) {
    for (auto& o: system->objects) {
        o->acc = o->F / o->mass;
        o->pos = o->pos + (o->vel * dt);
        o->vel = o->vel + (o->acc * dt);
    }
}