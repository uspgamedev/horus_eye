#ifndef HORUSEYE_GAME_UTILS_GEOMETRYPRIMITIVES_H_
#define HORUSEYE_GAME_UTILS_GEOMETRYPRIMITIVES_H_
#include "../../framework/vector2D.h"
using namespace framework;

namespace utils{
bool GPleft(Vector2D a, Vector2D b, Vector2D c);
bool GPintersect(Vector2D a, Vector2D b, Vector2D c, Vector2D d);
double GPdistance(Vector2D a,Vector2D b);
double GPdistance(int ax,int ay,int bx,int by);
}
#endif
