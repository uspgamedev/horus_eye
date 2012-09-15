#ifndef HORUSEYE_GAME_MAP_ROOM_H_
#define HORUSEYE_GAME_MAP_ROOM_H_

#include <list>
#include <vector>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include "game/scenes.h"
#include "game/sprites.h"
#include "game/map.h"

namespace map {

class Room {
  public:
    Room(const GameMap& matrix) : matrix_(matrix) {}
    ~Room() {}

    void AddObject(sprite::WorldObject*, const ugdk::Vector2D& position);
    void AddToWorld(scene::World*);

    const GameMap& matrix() const { return matrix_; }
    
  private:
    std::vector<sprite::WorldObject*> walls_;
    std::list<sprite::WorldObject*> objects_;
    ugdk::graphic::Node* floor_;
    GameMap matrix_;
    ugdk::Vector2D offset_;
};

} // namespace map

#endif /* HORUSEYE_GAME_MAP_ROOM_H_ */
