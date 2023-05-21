#include "object.h"
#include "system.h"
#include <iostream>

// Base class implementation of a point mass
void Object::applyForce(Vector2d F_) {
    if (mass <= 0) return;
    F = F + F_;
}

void Object::applyForce(double xmag, double ymag) {
    F.x += xmag;
    F.y += ymag;
}

void Object::applyForceAtPoint(Vector2d F_, Vector2d R_) {
    
}

void Object::render(SDL_Renderer* rend) {
    SDL_Rect rect;
    rect.x = pos.x*100-16*mass;
    rect.y = pos.y*100-16*mass;
    rect.w = 32*mass;
    rect.h = 32*mass;
    SDL_SetRenderDrawColor(rend, R,G,B,A);
    SDL_RenderFillRect(rend, &rect);
}

void Object::renderCircle(SDL_Renderer* rend)
{   
    int x = pos.x*100;
    int y = pos.y*100;
    int radius = 16*mass;
    SDL_SetRenderDrawColor(rend, R, G, B, A);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(rend, x + dx, y + dy);
            }
        }
    }
}

double Object::calculateEnergy() {
    energy = -mass * System::g * pos.y + 0.5 * mass * mag(vel) * mag(vel);
    return energy;
}

void Object::setColor(int r, int g, int b, int a) {
    R = r;
    G = g;
    B = b;
    A = a;
}