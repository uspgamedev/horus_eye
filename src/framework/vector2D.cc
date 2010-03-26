//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/vector2D.cc
// Implementacao da classe Vector2D.
//
#include <math.h>
#include "vector2D.h"

namespace framework {

// Member functions

// Devolve a norma do vetor
float Vector2D::length() const {
    return sqrt((x * x) + (y * y));
}

// Metodos estaticos

// Devolve a + b
Vector2D Vector2D::Add(Vector2D a, Vector2D b) {
    Vector2D ret;
    ret.x = (a.x + b.x);
    ret.y = (a.y + b.y);
    return ret;
}

// Devolve a - b
Vector2D Vector2D::Subtract(Vector2D a, Vector2D b) {
    Vector2D ret;
    ret.x = (a.x - b.x);
    ret.y = (a.y - b.y);
    return ret;
}

// Devolve scalar . a
Vector2D Vector2D::Multiply(Vector2D a, float scalar) {
    Vector2D ret;
    ret.x = (a.x * scalar);
    ret.y = (a.y * scalar);
    return ret;
}

// Devolve <a,b>
float Vector2D::InnerProduct(Vector2D a, Vector2D b) {
    return (a.x * b.x) + (a.y * b.y);
}

// Devolve o vetor a normalizado
Vector2D Vector2D::Normalized(Vector2D a) {
    Vector2D ret;
    ret.x = (a.x / a.length());
    ret.y = (a.y / a.length());
    return ret;
}

//Overload de operadores
Vector2D operator+(const Vector2D& left_value,
                   const Vector2D& right_value) {
    return Vector2D::Add(left_value, right_value);
}

Vector2D operator-(const Vector2D& left_value,
                   const Vector2D& right_value) {
    return Vector2D::Subtract(left_value, right_value);
}


Vector2D operator*(const Vector2D& left_value,
                   const float& scalar) {
    return Vector2D::Multiply(left_value, scalar);
}

}  // namespace framework
