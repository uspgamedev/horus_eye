#ifndef HORUSEYE_GAME_UTILS_HUD_H_
#define HORUSEYE_GAME_UTILS_HUD_H_

#include "../../framework/layer.h"
#include "../scenes/world.h"

namespace framework {
class Sprite;
class Animation;
}

using framework::Sprite;
using framework::Animation;

namespace utils {
class Hud: public framework::Layer {
  public:
    Hud(scene::World*);
    virtual ~Hud();
    
    virtual void Update(float delta_t);

  private:
    Sprite* life_bar_;
    Sprite* mana_bar_;
    Sprite* totem_[2];
    Image* weapon_icon_;
    map<Image*, Sprite*> icon_added;

    Sprite *enemy_counter_[7], *fps_meter_[3];
    framework::uint32 enemy_counter_value_[7], fps_meter_value_[3];
};

}

#endif /* HORUSEYE_GAME_UTILS_HUD_H_ */
