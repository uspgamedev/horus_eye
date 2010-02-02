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
float Vector2D::length() const
{
//    return sqrt(Vector2D::InnerProduct(this, this));
    return sqrt((x_ * x_) + (y_ * y_));
}

// Metodos estaticos

// Devolve a + b
Vector2D Vector2D::Add(Vector2D a, Vector2D b)
{
    Vector2D c;
    c.set_x(a.x() + b.x());
    c.set_y(a.y() + b.y());
    return c;
}
// Devolve a - b
Vector2D Vector2D::Subtract(Vector2D a, Vector2D b)
{
    Vector2D c;
    c.set_x(a.x() - b.x());
    c.set_y(a.y() - b.y());

    return c;
}
// Devolve scalar . a
Vector2D Vector2D::Multiply(Vector2D a, float scalar)
{
    Vector2D c;
    c.set_x(a.x() * scalar);
    c.set_y(a.y() * scalar);
    return c;
}
// Devolve <a,b>
float Vector2D::InnerProduct(Vector2D a, Vector2D b)
{
    return (a.x() * b.x()) + (a.y() * b.y());
}
// Devolve o vetor a normalizado
Vector2D Vector2D::Normalized(Vector2D a)
{
    Vector2D c;
    c.set_x(a.x() / a.length());
    c.set_y(a.y() / a.length());
    return c;
}

}  // namespace framework
