#ifndef HORUSEYE_GAME_CORE_WORLD_H_
#define HORUSEYE_GAME_CORE_WORLD_H_

#include <ugdk/system/taskplayer.h>
#include <ugdk/math/geometry.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/structure/intervalkdtree.h>
#include <ugdk/script/virtualobj.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/collision/collisionmanager.h>

#include "game/sprites.h"
#include "game/core.h"
#include "game/map.h"
#include "game/campaigns.h"
#include "game/components.h"
#include "game/core/gamelayer.h"
#include "game/resources/resource.h"

#include <list>
#include <set>
#include <vector>
#include <queue>
#include <unordered_map>

using ugdk::math::Vector2D;

namespace core {

class World : public ugdk::system::TaskPlayer {
  public:
    explicit World(const ugdk::math::Integer2D& size, const ugdk::script::VirtualObj&);
    ~World();

    void set_background_music(const std::string& music) { background_music_ = music; }
    void SetHero(const sprite::WObjPtr& hero);
    void QueueRoomChange(const sprite::WObjWeakPtr&, map::Room* next_room);

    void Start(campaigns::Campaign*);
    void End();

    void AddRoom(map::Room* room);
    void ChangeFocusedRoom(const std::string& name);
    void ChangeFocusedRoom(map::Room*);
    bool IsRoomActive(const std::string& name) const;
    bool IsRoomActive(const map::Room*) const;

    map::Room* FindRoomFromPoint(const ugdk::math::Vector2D& point) const;
    const map::Room* GetRoom(const std::string& name) const { return findRoom(name); }

    //getters
    const ugdk::math::Integer2D& size() const { return size_; }
    sprite::WObjWeakPtr hero() const { return hero_;  }
    const std::vector<map::Room*>& active_rooms() const { return active_rooms_; }
    const std::string& background_music() const { return background_music_; }

    pyramidworks::collision::CollisionManager* collision_manager() { return &collision_manager_; }
    pyramidworks::collision::CollisionManager* visibility_manager() { return &visibility_manager_; }

    // View
    const ugdk::math::Geometry& camera() const { return camera_; }

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
    sprite::WObjPtr hero_;
    ugdk::script::VirtualObj vobj_;
    std::string background_music_;

    // Graphic
    ugdk::math::Geometry camera_;

};  // class World

}  // namespace core

#endif  // HORUSEYE_GAME_CORE_WORLD_H_
