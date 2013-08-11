#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <list>
#include <vector>
#include <unordered_map>
#include <ugdk/action/scene.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>
#include <pyramidworks/collision.h>

#include "game/map.h"
#include "game/components.h"
#include "game/scenes/gamelayer.h"
#include "game/utils/levelmanager.h"
#include "game/resources/resource.h"

namespace utils {
class Hud;
}
namespace sprite {
class WorldObject;
}
using ugdk::math::Vector2D;

namespace scene {

#define WORLD() (utils::LevelManager::reference()->current_level() )

class World : public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    World(const ugdk::math::Integer2D& size);
    virtual ~World();

    void set_hero_initial_data(const std::string& room, const ugdk::math::Vector2D& pos) { 
        hero_initial_room_ = room; hero_initial_position_ = pos;
    }
    void SetHero(sprite::WorldObject *hero);

    void FinishLevel(utils::LevelManager::LevelState state) {
        level_state_ = state;
    }

    void Start();
    void End();

    void Focus();
    void DeFocus();

    void AddRoom(map::Room* room);
    void ActivateRoom(const std::string& name);
    void DeactivateRoom(const std::string& name);
    bool IsRoomActive(const std::string& name) const;
    bool IsRoomActive(const map::Room*) const;

    const map::Room* GetRoom(const std::string& name) const { return findRoom(name); }

    //getters
    sprite::WorldObject * hero() const { return hero_; }

    const ugdk::math::Integer2D& size() const { return size_; }
    const std::list<map::Room*>& active_rooms() const { return active_rooms_; }

    ugdk::graphic::Node* content_node() const { return content_node_; }

    pyramidworks::collision::CollisionManager* collision_manager() { return collision_manager_; }
    pyramidworks::collision::CollisionManager* visibility_manager() { return visibility_manager_; }
    
    //setters
    void set_size(const ugdk::math::Integer2D& _size) { size_ = _size; }

    ugdk::graphic::Node** layers() {
        return layers_;
    }

    ugdk::graphic::Node* layer_node(GameLayer layer) { 
        return layers_[layer];
    }

  protected:
    void SetupCollisionManager();
    map::Room* findRoom(const std::string& name) const;
    bool updateRooms(double dt);
    void removeAllRooms();

    sprite::WorldObject *hero_;

    utils::Hud *hud_;
    ugdk::math::Integer2D size_;
    std::unordered_map<std::string, map::Room*> rooms_;
    std::list<map::Room*> active_rooms_;

  private:
    utils::LevelManager::LevelState level_state_;
    pyramidworks::collision::CollisionManager* collision_manager_;
    pyramidworks::collision::CollisionManager* visibility_manager_;
    ugdk::graphic::Node* content_node_;
    ugdk::graphic::Node *layers_[2];

    std::string hero_initial_room_;
    ugdk::math::Vector2D hero_initial_position_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
