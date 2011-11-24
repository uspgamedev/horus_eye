#ifndef HORUSEYE_GAME_SPRITE_BUTTON_H_
#define HORUSEYE_GAME_SPRITE_BUTTON_H_

#include <ugdk/time/timeaccumulator.h>
#include "game/sprites/scenery/floor.h"

namespace scene {
class World;
}

namespace sprite {

class Button : public Floor {
  private: typedef Floor super;
  public:
    Button(ugdk::Image* image, scene::World *world);
    ~Button() { delete reactive_time_; }

    virtual void Update(float delta_t);

    virtual void Press();
    virtual void DePress();

  private:
    ugdk::TimeAccumulator *reactive_time_;
    bool pressed_;
    scene::World *world_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_BUTTON_H_ */
