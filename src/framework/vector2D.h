//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/vector2D.h
// Definicao da classe Vector2D.
//
#ifndef HORUSEYE_FRAMEWORK_VECTOR2D_H_
#define HORUSEYE_FRAMEWORK_VECTOR2D_H_

namespace framework {

// Vetores de 2 dimensoes.
// As entradas do vetor sao valores float.
class Vector2D {
  public:
    // Construtores e destrutores
    Vector2D() : x(0.0), y(0.0) { }
    Vector2D(float x, float y) : x(x), y(y) { }
    ~Vector2D() { }


    // Member functions
    float length() const;

    // Metodos estaticos
    static Vector2D Add(Vector2D a, Vector2D b);
    static Vector2D Subtract(Vector2D a, Vector2D b);
    static Vector2D Multiply(Vector2D a, float scalar);
    static float InnerProduct(Vector2D a, Vector2D b);
    static Vector2D Normalized(Vector2D a);

    float x, y;
};

// Overload de operadores
Vector2D operator+(const Vector2D& left_value,
                   const Vector2D& right_value);
Vector2D operator-(const Vector2D& left_value,
                   const Vector2D& right_value);
Vector2D operator*(const Vector2D& left_value,
                   const float& scalar);

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_VECTOR2D_H_
