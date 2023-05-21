#include "doublependulum.h"
#include <cmath>

LagrangianDoublePendulum::LagrangianDoublePendulum() {
    pivot = Vector2d(3.5, 1.5);
    updateVectors();
}

LagrangianDoublePendulum::~LagrangianDoublePendulum() {

}

void LagrangianDoublePendulum::setAngles(double t1, double t2) {
    theta1 = t1 * M_PI/180;
    theta2 = t2 * M_PI/180;
    updateVectors();
}

void LagrangianDoublePendulum::setMassRatio(double ratio) {
    m1 = ratio;
    m2 = 1;
}

void LagrangianDoublePendulum::clear() {
    theta1 = 0;
    theta1dot = 0;
    theta1ddot = 0;
    theta2 = 0;
    theta2dot = 0;
    theta2ddot = 0;
}

void LagrangianDoublePendulum::update() {
    theta1ddot=(-g*(2*m1+m2)*sin(theta1)-m2*g*sin(theta1-2*theta2)-2*sin(theta1-theta2)*m2*(r2*theta2dot*theta2dot+r1*cos(theta1-theta2)*theta1dot*theta1dot))/(r1*(2*m1+m2-m2*cos(2*theta1-2*theta2)));
    theta2ddot=(2*sin(theta1-theta2)*((m1+m2)*r1*theta1dot*theta1dot+g*(m1+m2)*cos(theta1)+r2*m2*cos(theta1-theta2)*theta2dot*theta2dot))/(r2*(2*m1+m2-m2*cos(2*theta1-2*theta2)));

    
    theta1 += dt * theta1dot;
    theta2 += dt * theta2dot;
    theta1dot += dt * theta1ddot;
    theta2dot += dt * theta2ddot;

    updateVectors();
}

void LagrangianDoublePendulum::setColor(int r, int g, int b, int a) {
    R = r;
    G = g;
    B = b;
    A = a;
}

void LagrangianDoublePendulum::drawDircle(SDL_Renderer* rend, int x, int y, int r) {
    SDL_SetRenderDrawColor(rend, R, G, B, A);
    for (int w = 0; w < r * 2; w++)
    {
        for (int h = 0; h < r * 2; h++)
        {
            int dx = r - w; // horizontal offset
            int dy = r - h; // vertical offset
            if ((dx*dx + dy*dy) <= (r * r))
            {
                SDL_RenderDrawPoint(rend, x + dx, y + dy);
            }
        }
    }
}

void LagrangianDoublePendulum::render(SDL_Renderer* rend) {
    SDL_SetRenderDrawColor(rend, 133, 133, 133, 255);
    SDL_RenderDrawLine(rend, pivot.x*100, pivot.y*100, string1.x*100, string1.y*100);
    SDL_RenderDrawLine(rend, string1.x*100, string1.y*100, string2.x*100, string2.y*100);
    drawDircle(rend, string1.x*100, string1.y*100, 16);
    drawDircle(rend, string2.x*100, string2.y*100, 16);
}

void LagrangianDoublePendulum::updateVectors() {
    string1.x = pivot.x + r1*sin(theta1);
    string1.y = pivot.y + r1*cos(theta1);
    string2.x = string1.x + r2*sin(theta2);
    string2.y = string1.y + r2*cos(theta2);
}

std::vector<double> LagrangianDoublePendulum::getState() {
    std::vector<double> result;
    result.push_back(m1);
    result.push_back(string1.x);
    result.push_back(r1 * cos(theta1) * theta1dot);
    result.push_back(string1.y);
    result.push_back(r1 * -1 * sin(theta1) * theta1dot);
    result.push_back(m2);
    result.push_back(string2.x);
    result.push_back(r1 * cos(theta1) * theta1dot + r2 * theta2dot * cos(theta2));
    result.push_back(string2.y);
    result.push_back(r1 * -1 * sin(theta1) * theta1dot - r2 * theta2dot * sin(theta2));

    return result;
}