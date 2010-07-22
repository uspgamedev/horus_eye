
#include"geometryprimitives.h"
#include<cmath>
namespace utils{
bool GPleft(Vector2D a, Vector2D b, Vector2D c){
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) > 0;
}

bool GPintersect(Vector2D a, Vector2D b, Vector2D c, Vector2D d) {
    if ((GPleft(a, b, c) != GPleft(a, b, d)) && (GPleft(c, d, a) != GPleft(c, d, b))) return true;
    return false;
}

double GPdistance(Vector2D a,Vector2D b){
    return sqrt((a.x-b.x) * (a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

double GPdistance(int ax,int ay,int bx,int by){
    return sqrt((ax-bx) * (ax-bx) + (ay-by) * (ay-by));
}
}
