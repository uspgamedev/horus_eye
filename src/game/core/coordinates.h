#ifndef HORUSEYE_GAME_CORE_COORDINATES_H_
#define HORUSEYE_GAME_CORE_COORDINATES_H_

#include <ugdk/math/vector2D.h>

namespace core {

// Funcao auxiliar que transforma VETORES de coordenadas de tela para de mundo
/** Converts a vector into world linear coordinates, from a screen-coordinates based vector.
  * For reference, the Vector (TS.x/2.0; -TS.x/2.0) converts into (1.0; 0.0) */
ugdk::Vector2D FromScreenLinearCoordinates(const ugdk::Vector2D& screen_coords);

// Funcao auxiliar que transforma VETORES de coordenadas de mundo para de tela
/** Converts a vector into screen linear coordinates, from a world-coordinates based vector.
  * For reference, the Vector (1.0; 0.0) converts into (TS.x/2.0; -TS.x/2.0) */
ugdk::Vector2D FromWorldLinearCoordinates(const ugdk::Vector2D& world_coords);

// Funcao que transforma PONTOS de coordenadas de mundo para de tela
ugdk::Vector2D FromWorldCoordinates(const ugdk::Vector2D& screen_coords);

// Funcao que transforma PONTOS de coordenadas de tela para de mundo
ugdk::Vector2D FromScreenCoordinates(const ugdk::Vector2D& screen_coords);

ugdk::Vector2D ConvertLightRadius(double radius);

} // namespace core

#endif  // HORUSEYE_GAME_CORE_COORDINATES_H_
