#ifndef HORUSEYE_GAME_MAP_ROOM_H_
#define HORUSEYE_GAME_MAP_ROOM_H_

#include <list>
#include <vector>
#include <string>
#include <queue>

#include <ugdk/portable/tr1.h>
#include FROM_TR1(unordered_map)

#include <ugdk/action/scene.h>
#include <ugdk/graphic.h>
#include <ugdk/math/integer2D.h>

#include "game/scenes.h"
#include "game/sprites.h"
#include "game/map.h"
#include "game/scenes/gamelayer.h"

namespace map {

class Room {
  public:
    Room(const std::string& name, const ugdk::math::Integer2D& size, 
        const ugdk::math::Integer2D& position, const GameMap& matrix);
    ~Room();

    void AddObject(sprite::WorldObject*, const ugdk::Vector2D& position);

    void Update(double dt);

    sprite::WorldObject* WorldObjectByTag (const std::string& tag);
    void RemoveTag(const std::string& tag);

    const std::string& name() const { return name_; }
    const GameMap& matrix() const { return matrix_; }
    const ugdk::math::Integer2D& size() const { return size_; }

    ugdk::graphic::Node* layer_node(scene::GameLayer layer) { 
        return layers_[layer];
    }
    
  private:
    void UpdateObjects(double delta_t);
    void DeleteToBeRemovedObjects();
    void FlushObjectQueue();

    typedef std::tr1::unordered_map<std::string, sprite::WorldObject*> TagTable;

    std::string name_;
    GameMap matrix_;
    ugdk::math::Integer2D size_, position_;
    TagTable tagged_;
    ugdk::graphic::Node *layers_[2];

    std::list<sprite::WorldObject*> objects_;
    std::queue<sprite::WorldObject*> queued_objects_;
};

} // namespace map

#endif /* HORUSEYE_GAME_MAP_ROOM_H_ */
