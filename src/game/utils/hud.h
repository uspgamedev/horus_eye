#ifndef HORUSEYE_GAME_UTILS_HUD_H_
#define HORUSEYE_GAME_UTILS_HUD_H_

#include <ugdk/action/entity.h>
#include "game/scenes/world.h"

namespace ugdk {
class Sprite;
class Text;
class Animation;
class Modifier;
class Node;
}

using ugdk::Sprite;
using ugdk::Animation;

namespace utils {
class Hud: public ugdk::Entity {
  public:
    Hud(scene::World*);
    virtual ~Hud();
    
    virtual void Update(float delta_t);
    ugdk::Node* node() { return node_; }

  private:
    ugdk::Node *node_;
    Image* weapon_icon_;
    map<Image*, Sprite*> icon_added;
    ugdk::Modifier *life_modifier_, *mana_modifier_, *block_modifier_;

    Sprite *enemy_counter_[7];
    ugdk::uint32 enemy_counter_value_[7];

#ifdef DEBUG
    Sprite *fps_meter_[3];
    ugdk::uint32 fps_meter_value_[3];
#endif
};

}

#endif /* HORUSEYE_GAME_UTILS_HUD_H_ */
