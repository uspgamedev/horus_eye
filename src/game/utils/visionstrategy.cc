#include "visionstrategy.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/geometry/convexpolygon.h>

#include "game/scenes/world.h"
#include "game/map/tile.h"
#include "game/map/room.h"
#include "game/constants.h"
#include "game/sprites/worldobject.h"
#include "game/context.h"

using namespace scene;
using namespace utils;
using namespace sprite;
using namespace ugdk;

using std::queue;
using map::GameMap;

bool solid(char obj){
    if(obj == WALL) return true;
    if(obj == DOOR) return true;
    return false;
}

bool wall(char obj){
    if(obj == WALL) return true;
    return false;
}
    
bool VisionStrategy::IsVisible(const ugdk::math::Vector2D& from, const ugdk::math::Vector2D& target) {
    using pyramidworks::collision::CollisionManager;
    using pyramidworks::geometry::ConvexPolygon;
    std::vector<Vector2D> points(2);
    points[1] = Vector2D(target - from);
    ConvexPolygon ray(points);

    // TODO: add support to UGDK to a simple "CHECK IF EXISTS"
    std::vector<sprite::WObjRawPtr> objects;
    context::GetCollidingVisibilityObjects("Opaque", ray, from, objects);
    return objects.empty();
}
    
bool VisionStrategy::IsVisible(sprite::WObjRawPtr from) {
    WObjPtr hero = from->current_room()->level()->hero().lock();
    if(hero)
        return IsVisible(from, hero->world_position());
    else
        return IsVisible(from, from->world_position());
}

bool VisionStrategy::IsVisible(sprite::WObjRawPtr from, const ugdk::math::Vector2D& position2) {
    return IsVisible(from->world_position(), position2);
}

#define TO_MATRIX(value) static_cast<int>(value + 0.5)

bool VisionStrategy::IsLightVisible(Vector2D position1, Vector2D position2) {
    /*
    World *world = WORLD();
    const GameMap& matrix = world->room()->matrix();

    if(position2.x < 0.0){
        WorldObject* hero = world->hero_world_object();
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
    */

    return true;
}

queue<Vector2D> VisionStrategy::Calculate(sprite::WObjRawPtr who) {
    auto hero = who->current_room()->level()->hero().lock();

    queue<Vector2D> resp;

    if(!hero)
        return resp;

    if(IsVisible(who, hero->world_position()))
        resp.push(hero->world_position());
    return resp;
}

