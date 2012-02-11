#ifndef HORUSEYE_GAME_UTILS_HUD_H_
#define HORUSEYE_GAME_UTILS_HUD_H_

#include <ugdk/action/entity.h>
#include <ugdk/graphic.h>
#include "game/scenes/world.h"

namespace skills {
class Skill;
} // skills

namespace utils {
class Hud: public ugdk::Entity {
  public:
    Hud(scene::World*);
    virtual ~Hud();
    
    virtual void Update(double delta_t);
    ugdk::graphic::Node* node() { return node_; }

  private:
    ugdk::graphic::Node *node_, *weapon_icon_, *mummy_counter_text_holder_;
    int previous_mummy_counter_value_;

    skills::Skill *displayed_skill_;

    ugdk::graphic::Modifier *life_modifier_, *mana_modifier_, *block_modifier_;

#ifdef DEBUG
    ugdk::graphic::Node* fps_meter_node_;
    int previous_fps_;
#endif
};

}

#endif /* HORUSEYE_GAME_UTILS_HUD_H_ */
