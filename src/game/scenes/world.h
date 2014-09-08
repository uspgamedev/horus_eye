#ifndef HORUSEYE_GAME_SCENES_WORLD_H_
#define HORUSEYE_GAME_SCENES_WORLD_H_

#include <ugdk/action/scene.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/script/virtualobj.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/sprites.h"
#include "game/scenes.h"
#include "game/map.h"
#include "game/campaigns.h"
#include "game/components.h"
#include "game/scenes/gamelayer.h"
#include "game/resources/resource.h"

#include <list>
#include <set>
#include <vector>
#include <queue>
#include <unordered_map>

namespace utils {
class Hud;
}
using ugdk::math::Vector2D;

namespace scene {

//#define WORLD() (utils::LevelManager::reference()->current_level() )

class World : public ugdk::action::Scene {
  typedef ugdk::action::Scene super;
  public:
    explicit World(const ugdk::math::Integer2D& size, const ugdk::script::VirtualObj&);
    virtual ~World();

    void set_light_rendering(LightRendering* lr) { light_rendering_ = lr; }
    void set_hero_initial_data(const std::string& room, const ugdk::math::Vector2D& pos) { 
        hero_initial_room_ = room; hero_initial_position_ = pos;
    }
    void SetHero(const sprite::WObjPtr& hero);
    void QueueRoomChange(const sprite::WObjWeakPtr&, map::Room* next_room);

    void Start(campaigns::Campaign*);
    void End();
    ugdk::action::Scene* CreateLightRenderingScene();

    void Focus();
    void DeFocus();

    void RenderLight(ugdk::graphic::Canvas& canvas) const;

    void AddRoom(map::Room* room);
    void ChangeFocusedRoom(const std::string& name);
    void ChangeFocusedRoom(map::Room*);
    bool IsRoomActive(const std::string& name) const;
    bool IsRoomActive(const map::Room*) const;

    map::Room* FindRoomFromPoint(const ugdk::math::Vector2D& point) const;
    const map::Room* GetRoom(const std::string& name) const { return findRoom(name); }

    //getters
    const ugdk::math::Integer2D& size() const { return size_; }
    const ugdk::graphic::Geometry& camera() const { return camera_; }
    sprite::WObjWeakPtr hero() const { return hero_;  }
    LightRendering* light_rendering() const { return light_rendering_; }
    //const std::set<map::Room*>& active_rooms() const { return active_rooms_; }

    pyramidworks::collision::CollisionManager* collision_manager() { return &collision_manager_; }
    pyramidworks::collision::CollisionManager* visibility_manager() { return &visibility_manager_; }

  protected:
    void SetupCollisionManager();
    map::Room* findRoom(const std::string& name) const;
    bool updateRooms(double dt);
    void removeAllRooms();

    // World Layout
    ugdk::math::Integer2D size_;
    std::unordered_map<std::string, map::Room*> rooms_;
    std::vector<map::Room*> active_rooms_;
    ugdk::structure::ikdtree::IntervalKDTree<map::Room*, 2> rooms_by_location_;

  private:
    // Game logic
    campaigns::Campaign* campaign_;
    pyramidworks::collision::CollisionManager collision_manager_;
    pyramidworks::collision::CollisionManager visibility_manager_;
    std::queue<std::pair<sprite::WObjWeakPtr, map::Room*> > queued_moves_;
    ugdk::script::VirtualObj vobj_;
    LightRendering* light_rendering_;

    // Graphic
    utils::Hud *hud_;
    ugdk::graphic::Geometry camera_;

    // Hero
    sprite::WObjPtr hero_;
    std::string hero_initial_room_;
    ugdk::math::Vector2D hero_initial_position_;

};  // class World

}  // namespace scene

#endif  // HORUSEYE_GAME_SCENES_WORLD_H_
