#ifndef HORUSEYE_GAME_UTILS_HUD_H_
#define HORUSEYE_GAME_UTILS_HUD_H_

#include "ugdk/layer.h"
#include "../scenes/world.h"

namespace ugdk {
class Sprite;
class Text;
class Animation;
class Modifier;
}

using ugdk::Sprite;
using ugdk::Animation;

namespace utils {
class Hud: public ugdk::Layer {
  public:
    Hud(scene::World*);
    virtual ~Hud();
    
    virtual void Update(float delta_t);

  private:
    Sprite* life_bar_;
    Sprite* mana_bar_;
    Image* weapon_icon_;
    map<Image*, Sprite*> icon_added;
    ugdk::Modifier *life_modifier_, *mana_modifier_;

    Sprite *enemy_counter_[7], *fps_meter_[3];
    ugdk::uint32 enemy_counter_value_[7], fps_meter_value_[3];
};

}

#endif /* HORUSEYE_GAME_UTILS_HUD_H_ */
