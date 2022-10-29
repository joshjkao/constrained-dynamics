#ifndef VECTOR_2D
#define VECTOR_2D
#include <iostream>

struct Vector2d {
    double x;
    double y;

    Vector2d(double i = 0, double j = 0);

    Vector2d& add(const Vector2d& other);
    Vector2d& subtract(const Vector2d& other);
    Vector2d& abs();

    Vector2d& multiply(const double& other);
    Vector2d& divide(const double& other);

    friend double mag(const Vector2d& vec);
    friend Vector2d proj(const Vector2d& B, const Vector2d& A);
    friend Vector2d hat(const Vector2d& A);

    Vector2d& operator*=(const double& other);

    friend Vector2d operator+(const Vector2d& LHS, const Vector2d& RHS);
    friend Vector2d operator-(const Vector2d& LHS, const Vector2d& RHS);

    friend Vector2d operator*(const Vector2d& LHS, const double& RHS);
    friend double operator*(const Vector2d& LHS, const Vector2d& RHS);
    friend Vector2d operator/(const Vector2d& LHS, const double& RHS);


    friend std::ostream& operator<<(std::ostream& os, const Vector2d& vec);
};

#endif