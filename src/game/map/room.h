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

namespace map {

static const bool POSITION_ABSOLUTE = true;
static const bool POSITION_RELATIVE = false;

class Room {
  public:
    Room(const std::string& name, const ugdk::math::Integer2D& size, 
        const ugdk::math::Integer2D& position, const GameMap& matrix);
    ~Room();

    void Update(double dt);

    /** Adds the object to the room. 
    Objects are added only at the end of a room's Update, unless the room hasn't been added to a World. */
    void AddObject(sprite::WorldObject*);

    /** Adds the object to the room, in the given position.
    @param obj The object to be added.
    @param position The position to place the object in.
    @param absolute Whether the position is relative to the room's position in the World, or absolute. Defaults to relative.
    @see AddObject */
    void AddObject(sprite::WorldObject* obj, const ugdk::Vector2D& position, bool absolute = POSITION_RELATIVE);

    /// Sets which level this room belongs to.
    void DefineLevel(scene::World*);

    /// Gets an object based on it's tag.
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
