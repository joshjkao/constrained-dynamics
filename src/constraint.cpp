#include "constraint.h"

DistanceConstraintFixed::DistanceConstraintFixed(Object* obj_, Vector2d pivot_):
obj(obj_), pivot(pivot_)
{

}

DistanceConstraintFixed::~DistanceConstraintFixed() {

}

void DistanceConstraintFixed::addJacobian(std::vector<double>& J, std::vector<double>& Jdot) {
    J[2*obj->index] = obj->pos.x - pivot.x;
    J[(2*obj->index)+1] = obj->pos.y - pivot.y;
    Jdot[2*obj->index] = obj->vel.x;
    Jdot[2*obj->index+1] = obj->vel.y;
}

void DistanceConstraintFixed::render(SDL_Renderer* rend) {
    SDL_SetRenderDrawColor(rend, 133, 133, 133, 255);
    SDL_RenderDrawLine(rend, pivot.x*100, pivot.y*100, obj->pos.x*100, obj->pos.y*100);
}

DistanceConstraint::DistanceConstraint(Object* obj1_, Object* obj2_):
obj1(obj1_), obj2(obj2_)
{

}

DistanceConstraint::~DistanceConstraint() {

}

void DistanceConstraint::addJacobian(std::vector<double>& J, std::vector<double>& Jdot) {
    J[2*obj1->index] = obj1->pos.x - obj2->pos.x;
    J[(2*obj1->index)+1] = obj1->pos.y - obj2->pos.y;
    Jdot[2*obj1->index] = obj1->vel.x - obj2->vel.x;
    Jdot[2*obj1->index+1] = obj1->vel.y - obj2->vel.y;

    J[2*obj2->index] = obj2->pos.x - obj1->pos.x;
    J[(2*obj2->index)+1] = obj2->pos.y - obj1->pos.y;
    Jdot[2*obj2->index] = obj2->vel.x - obj1->vel.x;
    Jdot[2*obj2->index+1] = obj2->vel.y - obj1->vel.y;
}

void DistanceConstraint::render(SDL_Renderer* rend) {
    SDL_SetRenderDrawColor(rend, 133, 133, 133, 255);
    SDL_RenderDrawLine(rend, obj1->pos.x*100, obj1->pos.y*100, obj2->pos.x*100, obj2->pos.y*100);
}