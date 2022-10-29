#include "forcegenerator.h"
#include "system.h"

ForceGenerator::ForceGenerator() {

}

ForceGenerator::~ForceGenerator() {

}

GravAttract::GravAttract(Object* A_, Object* B_):
A(A_), B(B_)
{

}

GravAttract::~GravAttract() {

}

void GravAttract::update() {
    Vector2d AtoB(B->pos.x - A->pos.x, B->pos.y - A->pos.y);
    AtoB = AtoB * B->mass * A->mass / 100;
    A->applyForce(AtoB);
    Vector2d BtoA = AtoB * -1;
    B->applyForce(BtoA);
}

Gravity::Gravity(Object* A_):
A(A_)
{

}

Gravity::~Gravity() {

}

void Gravity::update() {
    Vector2d grav(0,System::g);
    grav *= A->mass;
    A->applyForce(grav);
}


ConstRadiusFixedPivot::ConstRadiusFixedPivot(Object* A_, Object* B_):
A(A_), B(B_) 
{

}

ConstRadiusFixedPivot::~ConstRadiusFixedPivot() {
    
}

void ConstRadiusFixedPivot::update() {
    Vector2d AtoB(B->pos.x - A->pos.x, B->pos.y - A->pos.y);
    Vector2d BtoA = AtoB * -1;
    double lambda = (((A->F * -1) * BtoA) - ((A->vel * A->mass) * A->vel))/(BtoA * BtoA);
    Vector2d Fc = BtoA * lambda;
    A->applyForce(Fc);
}

ConstRadius::ConstRadius(Object* A_, Object* B_):
A(A_), B(B_)
{

}

ConstRadius::~ConstRadius() {

}

void ConstRadius::update() {
    Vector2d p =  A->pos - B->pos;
    Vector2d pdot = (A->vel - B->vel);
    double lambda = (((A->F * -1) * p) - ((pdot * A->mass) * pdot))/(p * p);
    A->applyForce(p * lambda);
    B->applyForce(p * -lambda);
}

Spring::Spring(Object* A_, Object* B_, double k_, double r0_):
A(A_), B(B_), k(k_), r0(r0_)
{

}

Spring::~Spring() {
    
}

void Spring::update() {
    Vector2d AtoB(B->pos.x - A->pos.x, B->pos.y - A->pos.y);
    double rDiff = mag(AtoB) - r0;
    double magnitude = k * rDiff * 0.9; // Added damping factor to stop simulation from exploding
    Vector2d AF = hat(AtoB) * magnitude;
    A->applyForce(AF);
    AF *= -1;
    B->applyForce(AF);
}

double Spring::calculateEnergy() {
    Vector2d AtoB(B->pos.x - A->pos.x, B->pos.y - A->pos.y);
    double rDiff = mag(AtoB) - r0;
    energy = 0.5 * k * rDiff * rDiff;
    return energy;
}