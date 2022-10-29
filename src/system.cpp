#include "system.h"
double System::g = 9.8;

// Vector operators
std::vector<double> operator-(const std::vector<double>& A, const std::vector<double>& B) {
    std::vector<double> solution;
    if (A.size() != B.size()) return solution;
    for (unsigned int i = 0; i < A.size(); i++) {
        solution.push_back(A[i] - B[i]);
    }
    return solution;
}

std::vector<double> operator+(const std::vector<double>& A, const std::vector<double>& B) {
    std::vector<double> solution;
    if (A.size() != B.size()) return solution;
    for (unsigned int i = 0; i < A.size(); i++) {
        solution.push_back(A[i] + B[i]);
    }
    return solution;
}

std::vector<double> operator*(const std::vector<double>& A, double D) {
    std::vector<double> solution;
    for (unsigned int i = 0; i < A.size(); i++) {
        solution.push_back(A[i]*D);
    }
    return solution;
}

std::ostream& operator<<(std::ostream& os, const std::vector<double>& A) {
    os << "< ";
    for (unsigned int i = 0; i < A.size(); i++) {
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
    for (int i = 0; i < 2*n; i++) {
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
    for (int i = 0; i < n; i++) {
        Q[2*i] = objects[i]->pos.x;
        Q[2*i+1] = objects[i]->pos.y;
        Qdot[2*i] = objects[i]->vel.x;
        Qdot[2*i+1] = objects[i]->vel.y;
        Fext[2*i] = objects[i]->F.x;
        Fext[2*i+1] = objects[i]->F.y;
    }
}

void System::calculateJacobian() {
    // if (constraints.size() == 0) return;
    updateState();
    J = Matrix(constraints.size(), n*2);
    Jdot = Matrix(constraints.size(), n*2);
    for (unsigned int i = 0; i < constraints.size(); i++) {
        constraints[i]->addJacobian(J[i], Jdot[i]);
    }
    // std::vector< std::vector<double> > j = {{Q[0]-5,Q[1]-1,0,0},
    //                                         {Q[0]-Q[2],Q[1]-Q[3],Q[2]-Q[0],Q[3]-Q[1]}};
    // J = Matrix(j);
    // std::vector< std::vector<double> > jdot = {{Qdot[0],Qdot[1],0,0},
    //                                            {Qdot[0]-Qdot[2],Qdot[1]-Qdot[3],Qdot[2]-Qdot[0],Qdot[3]-Qdot[1]}};
    // Jdot = Matrix(jdot);
    JT = transpose(J);
}

void System::applyConstraintForces() {
    // if (constraints.size() == 0) return;
    calculateJacobian();
    // Matrix LHS = J*Minv*JT;
    
    // std::vector<double> RHS = (Jdot*Qdot)*-1 - J*Minv*Fext;

    // std::cout << LHS;

    // std::vector<double> lambda = LHS.gaussElim(RHS);

    Qddot = Minv*(Fext + (JT*((J*Minv*JT).gaussElim((Jdot*Qdot)*-1 - J*Minv*Fext))));
    // std::cout << Qddot << std::endl;
    int ind = 0;
    for (int i = 0; i < n; i++) {
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

void System::cleanup() {
    for (auto& o: objects) {
        delete o;
    }
    objects.clear();
    for (auto& g: generators) {
        delete g;
    }
    generators.clear();
}