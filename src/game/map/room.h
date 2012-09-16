#ifndef HORUSEYE_GAME_MAP_ROOM_H_
#define HORUSEYE_GAME_MAP_ROOM_H_

#include <list>
#include <vector>
#include <string>

#include <ugdk/portable/tr1.h>
#include FROM_TR1(unordered_map)

#include <ugdk/graphic.h>
#include <ugdk/math/integer2D.h>

#include "game/scenes.h"
#include "game/sprites.h"
#include "game/map.h"

namespace map {

class Room {
  public:
    Room(const ugdk::math::Integer2D& _size, const GameMap& matrix);
    ~Room();

    void AddObject(sprite::WorldObject*, const ugdk::Vector2D& position);
    void AddToWorld(scene::World*);

    sprite::WorldObject* WorldObjectByTag (const std::string& tag);
    void RemoveTag(const std::string& tag);
    void AddFloor(ugdk::graphic::Node* floor);

    const GameMap& matrix() const { return matrix_; }
    const ugdk::math::Integer2D& size() const { return size_; }
    
  private:
    typedef std::tr1::unordered_map<std::string, sprite::WorldObject*> TagTable;

    std::list<sprite::WorldObject*> objects_;
    ugdk::graphic::Node* floor_;
    GameMap matrix_;
    ugdk::math::Integer2D offset_, size_;
    TagTable tagged_;
};

} // namespace map

#endif /* HORUSEYE_GAME_MAP_ROOM_H_ */
