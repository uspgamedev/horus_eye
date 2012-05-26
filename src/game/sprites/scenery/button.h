#ifndef HORUSEYE_GAME_SPRITE_BUTTON_H_
#define HORUSEYE_GAME_SPRITE_BUTTON_H_

#include <ugdk/time/timeaccumulator.h>
#include "game/sprites/worldobject.h"

namespace scene {
class World;
}

namespace sprite {

class Button : public WorldObject {
  typedef WorldObject super;
  public:
    Button(ugdk::graphic::FlexibleSpritesheet* image, scene::World *world, double active_time = 1.0);
    ~Button() { delete reactive_time_; }

    virtual void Update(double delta_t);

    virtual void Die();

    virtual void Press();
    virtual void DePress();

    void set_world_position(const ugdk::Vector2D& pos);

  private:
    ugdk::time::TimeAccumulator *reactive_time_;
    bool pressed_;
    scene::World *world_;
    ugdk::graphic::Sprite *sprite_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_BUTTON_H_ */
