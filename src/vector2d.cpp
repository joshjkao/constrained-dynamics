#include "vector2d.h"
#include <cmath>
#include <iostream>

Vector2d::Vector2d(double i, double j)
{
    x = (double)i;
    y = (double)j;
}

Vector2d& Vector2d::add(const Vector2d& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

Vector2d& Vector2d::subtract(const Vector2d& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

Vector2d& Vector2d::abs() {
    this->x = std::abs(this->x);
    this->y = std::abs(this->y);
    return *this;
}

Vector2d& Vector2d::multiply(const double& other) {
    this->x *= other;
    this->y *= other;
    return *this;
}

Vector2d& Vector2d::divide(const double& other) {
    this->x /= other;
    this->y /= other;
    return *this;
}

double mag(const Vector2d& vec) {
    double result = vec.x * vec.x + vec.y * vec.y;
    result = pow(result, 0.5);
    return result;
}

Vector2d proj(const Vector2d& B, const Vector2d& A) {
    double c = A * B;
    c/= mag(B);
    c/= mag(B);
    Vector2d C(B);
    return C*c;
}

Vector2d hat(const Vector2d& A) {
    Vector2d B(A);
    B.divide(mag(A));
    return B;
}

Vector2d& Vector2d::operator*=(const double& other) {
    this->x *= other;
    this->y *= other;
    return *this;
}

Vector2d operator+(const Vector2d& LHS, const Vector2d& RHS) {
    Vector2d c(LHS);
    c.add(RHS);
    return c;
}

Vector2d operator-(const Vector2d& LHS, const Vector2d& RHS) {
    Vector2d c(LHS);
    c.subtract(RHS);
    return c;
}

Vector2d operator*(const Vector2d& LHS, const double& RHS) {
    Vector2d c(LHS);
    c.multiply(RHS);
    return c;
}

double operator*(const Vector2d& LHS, const Vector2d& RHS) {
    return LHS.x * RHS.x + LHS.y * RHS.y;
}

Vector2d operator/(const Vector2d& LHS, const double& RHS) {
    Vector2d c(LHS);
    c.divide(RHS);
    return c;
}

std::ostream& operator<<(std::ostream& os, const Vector2d& vec) {
    os << "<" << vec.x << ", " << vec.y << ">";
    return os;
}