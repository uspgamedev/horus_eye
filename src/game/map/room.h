#ifndef HORUSEYE_GAME_MAP_ROOM_H_
#define HORUSEYE_GAME_MAP_ROOM_H_

#include <list>
#include <vector>
#include <string>
#include <queue>

#include <ugdk/portable/tr1.h>
#include FROM_TR1(unordered_map)

#include <ugdk/graphic.h>
#include <ugdk/math/integer2D.h>

#include "game/scenes.h"
#include "game/sprites.h"
#include "game/map.h"
#include "game/scenes/gamelayer.h"

namespace map {
static const bool POSITION_ABSOLUTE = true;
static const bool POSITION_RELATIVE = false;

class Room {
  public:
    Room(const std::string& name, const ugdk::math::Integer2D& size, 
        const ugdk::math::Integer2D& position, const GameMap& matrix);
    ~Room();

    void Update(double dt);

    void AddObject(sprite::WorldObject*);
    void AddObject(sprite::WorldObject*, const ugdk::Vector2D& position, bool absolute = POSITION_RELATIVE);
    void DefineLevel(scene::World*);

    sprite::WorldObject* WorldObjectByTag (const std::string& tag) const;
    void RemoveTag(const std::string& tag);

    const std::string& name() const { return name_; }
    const GameMap& matrix() const { return matrix_; }
    const ugdk::math::Integer2D& size() const { return size_; }
    const ugdk::math::Integer2D& position() const { return position_; }
    ugdk::graphic::Node* floor() const { return floor_; }

    typedef std::list<sprite::WorldObject*>::const_iterator WObjListConstIterator;
    WObjListConstIterator begin() const { return objects_.begin(); } 
    WObjListConstIterator end() const { return objects_.end(); }

  private:
    void updateObjects(double delta_t);
    void deleteToBeRemovedObjects();
    void flushObjectQueue();
    void handleNewObject(sprite::WorldObject*);


    typedef std::tr1::unordered_map<std::string, sprite::WorldObject*> TagTable;

    std::string name_;
    GameMap matrix_;
    ugdk::math::Integer2D size_, position_;
    TagTable tagged_;
    ugdk::graphic::Node* floor_;

    scene::World* level_;
    std::list<sprite::WorldObject*> objects_;
    std::queue<sprite::WorldObject*> queued_objects_;
};

} // namespace map

#endif /* HORUSEYE_GAME_MAP_ROOM_H_ */
