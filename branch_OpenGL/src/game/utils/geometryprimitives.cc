
#include"geometryprimitives.h"
#include<cmath>
namespace utils{

/* verifica se o ponto c esta a esquerda do vetor a->b*/
bool GPleft(Vector2D a, Vector2D b, Vector2D c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0;
}

/* Verifica se duas retas, ab e cd se intersectam*/
bool GPintersect(Vector2D a, Vector2D b, Vector2D c, Vector2D d) {
    if ((GPleft(a, b, c) != GPleft(a, b, d)) && (GPleft(c, d, a) != GPleft(c, d, b))) return true;
    return false;
}

}//namespace
