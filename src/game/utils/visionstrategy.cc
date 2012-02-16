#include <cmath>
#include "visionstrategy.h"
#include "geometryprimitives.h"
#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "tile.h"
#include "game/utils/constants.h"

using namespace std;
using namespace scene;
using namespace utils;
using namespace sprite;
using namespace ugdk;

bool solid(char obj){
    if(obj == WALL) return true;
    if(obj == DOOR) return true;
    return false;
}

bool wall(char obj){
    if(obj == WALL) return true;
    return false;
}


bool VisionStrategy::IsVisible(Vector2D position1, Vector2D position2){
    World *world = WORLD();
    GameMap& matrix = world->level_matrix();

    if(position2.x < 0.0){
        Hero* hero = world->hero();
        position2 = hero->world_position();
    }

    Vector2D distance = position2 - position1;
    if(distance.length() > Constants::MUMMY_SIGHT_RANGE)
        return false;

    for (int i = 0; i < (int)matrix.size(); i++) {
        for (int j = 0; j < (int)matrix[i].size(); j++) {
            if(solid(matrix[i][j]->object())){
                double x = static_cast<double>(j);
                double y = static_cast<double>(matrix.size() - i - 1);

                Vector2D a = Vector2D(x - 0.5, y - 0.5);
                Vector2D b = Vector2D(x - 0.5, y + 0.5);
                Vector2D c = Vector2D(x + 0.5, y + 0.5);
                Vector2D d = Vector2D(x + 0.5, y - 0.5);
                if (GPintersect(a, b, position1, position2)) return false;
                if (GPintersect(b, c, position1, position2)) return false;
                if (GPintersect(c, d, position1, position2)) return false;
                if (GPintersect(d, a, position1, position2)) return false;
            }
        }
    }
    return true;
}

#define TO_MATRIX(value) static_cast<int>(value + 0.5)

bool VisionStrategy::IsLightVisible(Vector2D position1, Vector2D position2) {
    World *world = WORLD();
    GameMap& matrix = world->level_matrix();

    if(position2.x < 0.0){
        Hero* hero = world->hero();
        position2 = hero->world_position();
    }

    Vector2D distance = position2 - position1;
    if(distance.length() > Constants::MUMMY_SIGHT_RANGE)
        return false;

    // Matrix positions of the objects.
    Vector2D ipos1(TO_MATRIX(position1.x), matrix.size() - TO_MATRIX(position1.y) - 1);
    Vector2D ipos2(TO_MATRIX(position2.x), matrix.size() - TO_MATRIX(position2.y) - 1);
    // Normalized direction vector.
    Vector2D dir = Vector2D::Normalized(distance);
    // Step sign for walking through the matrix.
    int step_i = ipos2.y > ipos1.y ? 1 : (ipos2.y == ipos1.y ? 0 : -1),
        step_j = ipos2.x > ipos1.x ? 1 : (ipos2.x == ipos1.x ? 0 : -1);

    // Actual position when walking through the path, in both world position
    // and matrix position.
    Vector2D ij_pos = position1;
    int i = (int)ipos1.y,
        j = (int)ipos1.x;

    // Walks through the path.
    /*
    printf("pos1=[%d][%d]\n", (int)ipos1.y, (int)ipos1.x);
    printf("pos2=[%d][%d]\n", (int)ipos2.y, (int)ipos2.x);
    printf("step=[%d][%d]\n", step_i, step_j);
    printf("dir=(%f, %f])\n", dir.x, dir.y);
    */
    while (i != (int)ipos2.y || j != (int)ipos2.x) {
        //printf("Checking [%d][%d] --> %c\n", i, j, matrix[i][j]);
        if (wall(matrix[i][j]->object())) return false;
        // Distances to the next square: left/rigt or up/down.
        double dx = fabs((j + 0.5*step_j - ij_pos.x)/dir.x),
              dy = fabs(((matrix.size()-i-1) - 0.5*step_i - ij_pos.y)/dir.y),
              length;
        if (!step_i || (step_j && dx <= dy)) {
            j += step_j;
            length = dx;
        }
        if (!step_j || (step_i && dy <= dx)) {
            i += step_i;
            length = dy;
        }
        ij_pos = ij_pos + dir*length;
    }

    /*printf("pos1=[%d][%d]\n", (int)ipos1.y, (int)ipos1.x);
    printf("pos2=[%d][%d]\n", (int)ipos2.y, (int)ipos2.x);
    printf("height=%d\nproportion=%d\n", height, proportion);*/
    //printf("Checking [%d][%d] --> %c\n", i, j, matrix[i][j]);

    return true;
}

queue<Vector2D> VisionStrategy::Calculate(Vector2D position) {
    World *world = WORLD();
    Hero* hero = world->hero();

    queue<Vector2D> resp;

    if(hero == NULL)
        return resp;

    if(IsVisible(position,hero->world_position()))
        resp.push(hero->world_position());
    return resp;
}

