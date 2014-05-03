#ifndef HORUSEYE_GAME_UTILS_HUD_H_
#define HORUSEYE_GAME_UTILS_HUD_H_

#include <ugdk/action/entity.h>
#include <ugdk/graphic.h>
#include "game/scenes/world.h"
#include "game/config.h"

namespace skills {
class Skill;
} // skills

namespace utils {
class Hud: public ugdk::action::Entity {
  public:
    Hud(scene::World*);
    virtual ~Hud();
    
    virtual void Update(double delta_t);
    ugdk::graphic::Node* node() { return node_; }

  private:
    scene::World* world_;

    ugdk::graphic::Node *node_, *weapon_icon_, *mummy_counter_text_holder_;
    int previous_mummy_counter_value_;

    const skills::Skill *displayed_skill_;

    ugdk::graphic::Node *life_bar_, *mana_bar_, *block_bar_;

#ifdef HORUSEYE_DEBUG_TOOLS
    ugdk::graphic::Node* fps_meter_node_;
    double previous_fps_;
#endif
};

}

#endif /* HORUSEYE_GAME_UTILS_HUD_H_ */
