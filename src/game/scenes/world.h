#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <list>
#include <vector>
#include <ugdk/portable/tr1.h>
#include FROM_TR1(unordered_map)
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
using ugdk::Vector2D;

namespace scene {

#define WORLD() (utils::LevelManager::reference()->get_current_level() )

class World : public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    World();
    virtual ~World();

    void set_hero_initial_data(const std::string& room, const ugdk::Vector2D& pos) { 
        hero_initial_room_ = room; hero_initial_position_ = pos;
    }
    void SetHero(sprite::WorldObject *hero);

    void FinishLevel(utils::LevelManager::LevelState state) {
        level_state_ = state;
    }

    void SetupCollisionManager();

    void Start();
    void End();

    void AddRoom(map::Room* room);
    void ActivateRoom(const std::string& name);
    void DeactivateRoom(const std::string& name);
    bool IsRoomActive(const std::string& name) const;
    bool IsRoomActive(const map::Room*) const;

    const map::Room* GetRoom(const std::string& name) const { return findRoom(name); }

    // Funcao auxiliar que transforma VETORES de coordenadas de tela para de mundo
    static Vector2D FromScreenLinearCoordinates(const Vector2D& screen_coords);
    
    // Funcao auxiliar que transforma VETORES de coordenadas de mundo para de tela
    static Vector2D FromWorldLinearCoordinates(const Vector2D& world_coords);

    // Funcao que transforma PONTOS de coordenadas de mundo para de tela
    static Vector2D FromWorldCoordinates(const Vector2D& screen_coords);

    // Funcao que transforma PONTOS de coordenadas de tela para de mundo
    static Vector2D FromScreenCoordinates(const Vector2D& screen_coords);

    static const Vector2D ConvertLightRadius(double radius);

    //getters
    sprite::WorldObject * hero() const { return hero_; }

    const ugdk::math::Integer2D& size() const { return size_; }
    const std::list<map::Room*>& active_rooms() const { return active_rooms_; }

    pyramidworks::collision::CollisionManager* collision_manager() { return collision_manager_; }
    pyramidworks::collision::CollisionManager* visibility_manager() { return visibility_manager_; }
    
    //setters
    void set_size(const ugdk::math::Integer2D& _size) { size_ = _size; }

    ugdk::graphic::Node* layer_node(GameLayer layer) { 
        return layers_[layer];
    }

  protected:
    map::Room* findRoom(const std::string& name) const;
    bool updateRooms(double dt);
    void removeAllRooms();

    sprite::WorldObject *hero_;

    utils::Hud *hud_;
    ugdk::math::Integer2D size_;
    std::tr1::unordered_map<std::string, map::Room*> rooms_;
    std::list<map::Room*> active_rooms_;

  private:
    utils::LevelManager::LevelState level_state_;
    pyramidworks::collision::CollisionManager* collision_manager_;
    pyramidworks::collision::CollisionManager* visibility_manager_;
    ugdk::graphic::Node *layers_[2];

    std::string hero_initial_room_;
    ugdk::Vector2D hero_initial_position_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
