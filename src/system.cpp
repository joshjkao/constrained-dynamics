#include "system.h"
double System::g = 9.8;

// Vector operators
std::vector<double> operator-(const std::vector<double>& A, const std::vector<double>& B) {
    std::vector<double> solution;
    if (A.size() != B.size()) return solution;
    for (unsigned int i = 0; i < A.size(); ++i) {
        solution.push_back(A[i] - B[i]);
    }
    return solution;
}

std::vector<double> operator+(const std::vector<double>& A, const std::vector<double>& B) {
    std::vector<double> solution;
    if (A.size() != B.size()) return solution;
    for (unsigned int i = 0; i < A.size(); ++i) {
        solution.push_back(A[i] + B[i]);
    }
    return solution;
}

std::vector<double> operator*(const std::vector<double>& A, double D) {
    std::vector<double> solution;
    for (unsigned int i = 0; i < A.size(); ++i) {
        solution.push_back(A[i]*D);
    }
    return solution;
}

std::ostream& operator<<(std::ostream& os, const std::vector<double>& A) {
    os << "< ";
    for (unsigned int i = 0; i < A.size(); ++i) {
        os << A[i] << " ";
    }
    os << " >";
    return os;
}


// System functions
System::System() {
    
}

System::~System() {
    
}

void System::addObject(Object* object) {
    objects.push_back(object);

    Q.push_back(object->pos.x);
    Q.push_back(object->pos.y);
    Qdot.push_back(object->vel.x);
    Qdot.push_back(object->vel.y);
    Qddot.push_back(object->acc.x);
    Qddot.push_back(object->acc.y);
    Fext.push_back(object->F.x);
    Fext.push_back(object->F.y);

    std::vector<double> newRow1;
    std::vector<double> newRow2;
    for (int i = 0; i < 2*n; ++i) {
        newRow1.push_back(0);
        newRow2.push_back(0);
    }
    newRow1.push_back(1/object->mass);
    Minv.addRow(newRow1);
    newRow2.push_back(0);
    newRow2.push_back(1/object->mass);
    Minv.addRow(newRow2);
    object->index = n++;
}

void System::addConstraint(Constraint* constr) {
    constraints.push_back(constr);
    J = Matrix(constraints.size(), n*2);
    Jdot = Matrix(constraints.size(), n*2);
}

void System::addForceGenerator(ForceGenerator* gen) {
    generators.push_back(gen);
}

void System::clearForces() {
    for (auto& o: objects) {
        o->F *= 0;
        o->acc *= 0;
    }
}

void System::applyForce(Vector2d F_, Object* object) {
    object->applyForce(F_);
}

void System::render(SDL_Renderer* rend) {
    for (auto& o: objects) {
        o->render(rend);
    }
}

double System::calculateEnergy() {
    energy = 0;
    for (auto& o: objects) {
        energy += o->calculateEnergy();
    }
    for (auto& g: generators) {
        energy += g->calculateEnergy();
    }
    return this->energy;
}

void System::updateState() {
    for (int i = 0; i < n; ++i) {
        Q[2*i] = objects[i]->pos.x;
        Q[2*i+1] = objects[i]->pos.y;
        Qdot[2*i] = objects[i]->vel.x;
        Qdot[2*i+1] = objects[i]->vel.y;
        Fext[2*i] = objects[i]->F.x;
        Fext[2*i+1] = objects[i]->F.y;
    }
}

void System::calculateJacobian() {
    if (constraints.size() == 0) return;
    updateState();
    for (unsigned int i = 0; i < constraints.size(); ++i) {
        constraints[i]->addJacobian(J[i], Jdot[i]);
    }
    JT = transpose(J);
}

void System::applyConstraintForces() {
    if (constraints.size() == 0) return;
    calculateJacobian();

    Qddot = Minv*(Fext + (JT*((J*Minv*JT).gaussElim((Jdot*Qdot)*-1 - J*Minv*Fext))));
    int ind = 0;
    for (int i = 0; i < n; ++i) {
        objects[i]->acc.x = Qddot[ind++];
        objects[i]->acc.y = Qddot[ind++];
    }
}

void System::dump(std::ostream& os) {
    int ind = 0;
    for (auto& o: objects) {
        os << "Object " << ind << std::endl;
        os << "position: " << o->pos << std::endl;
        os << "velocity: " << o->vel << std::endl;
        os << "acceleration: " << o->acc << std::endl;
        os << "force: " << o->F << std::endl;
        ind++;
    }
}

std::vector<double> System::getState(std::vector<Object*>& objs) {
    std::vector<double> result;
    for (auto& o: objs) {
        result.push_back(o->mass);
        result.push_back(o->pos.x);
        result.push_back(o->vel.x);
        result.push_back(o->pos.y);
        result.push_back(o->vel.y);
    }
    return result;
}

void System::clear(std::vector<Object*>& objs) {
    for (auto& o: objs) {
        o->pos *= 0;
        o->vel *= 0;
        o->F *= 0;
    }
}

void System::setAngles(Object* origin, std::vector<Object*>& objs, double theta1, double theta2, double r1, double r2) {
    if (objs.size() != 2) return;
    objs[0]->pos.x = origin->pos.x + r1 * sin(theta1 * M_PI/180);
    objs[0]->pos.y = origin->pos.y + r1 * cos(theta1 * M_PI/180);
    objs[1]->pos.x = objs[0]->pos.x + r2 * sin(theta2 * M_PI/180);
    objs[1]->pos.y = objs[0]->pos.y + r2 * cos(theta2 * M_PI/180);
}

void System::setMassRatio(std::vector<Object*>& objs, double ratio) {
    if (objs.size() != 2) return;
    objs[0]->mass = ratio;
    objs[1]->mass = 1;
}

void System::cleanup() {
    for (auto& o: objects) {
        delete o;
    }
    objects.clear();
    for (auto& g: generators) {
        delete g;
    }
    generators.clear();
    for (auto& c: constraints) {
        delete c;
    }
    constraints.clear();
}