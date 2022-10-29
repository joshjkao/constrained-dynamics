#include "ODEsolver.h"

EulerSolver::EulerSolver() {

}

EulerSolver::~EulerSolver() {
    
}

void EulerSolver::start(System* system, double dt_) {
    dt = dt_;
}

void EulerSolver::iterate(System* system) {
    // for (int i = 0; i < system->n; i++) {
    //     system->Q[2*i] = system->objects[i]->pos.x = system->Qdot[2*i] * dt;
    //     system->Q[2*i+1] = system->objects[i]->pos.y = system->Qdot[2*i+1] * dt;
    //     system->Qdot[2*i] = system->Qddot[2*i] * dt;
    //     system->Qdot[2*i+1] = system->Qddot[2*i+1] * dt;
    // }
    for (auto& o: system->objects) {
        
        o->pos = o->pos + (o->vel * dt);
        o->vel = o->vel + (o->acc * dt);
        
    }
}