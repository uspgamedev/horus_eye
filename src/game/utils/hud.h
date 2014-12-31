#ifndef HORUSEYE_GAME_UTILS_HUD_H_
#define HORUSEYE_GAME_UTILS_HUD_H_

#include <ugdk/ui.h>
#include "game/core/world.h"
#include "game/config.h"

namespace skills {
class Skill;
} // skills

namespace utils {
class Hud {
  public:
    Hud(core::World*);
    virtual ~Hud();
    
    virtual void Update(double delta_t);
    ugdk::ui::Node* node() { return node_.get(); }

  private:
    core::World* world_;

    std::shared_ptr<ugdk::ui::Node> node_, weapon_icon_, mummy_counter_text_holder_;
    int previous_mummy_counter_value_;

    const skills::Skill *displayed_skill_;

    std::shared_ptr<ugdk::ui::Node> life_bar_, mana_bar_, block_bar_;

#ifdef HORUSEYE_DEBUG_TOOLS
    std::shared_ptr<ugdk::ui::Node> fps_meter_node_;
    double previous_fps_;
#endif
};

}

#endif /* HORUSEYE_GAME_UTILS_HUD_H_ */
