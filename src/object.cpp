#include "object.h"
#include "system.h"
#include <iostream>

void Object::applyForce(Vector2d F_) {
    if (mass <= 0) return;
    F = F + F_;
}

void Object::render(SDL_Renderer* rend) {
    SDL_Rect rect;
    rect.x = pos.x*100-16*mass;
    rect.y = pos.y*100-16*mass;
    rect.w = 32*mass;
    rect.h = 32*mass;
    SDL_SetRenderDrawColor(rend, 255,255,255,255);
    SDL_RenderFillRect(rend, &rect);
}

double Object::calculateEnergy() {
    energy = -mass * System::g * pos.y + 0.5 * mass * mag(vel) * mag(vel);
    return energy;
}