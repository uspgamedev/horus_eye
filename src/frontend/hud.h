#ifndef HORUSEYE_FRONTEND_HUD_H_
#define HORUSEYE_FRONTEND_HUD_H_

#include <ugdk/ui.h>
#include "game/config.h"
#include "game/skills.h"
#include "game/core.h"

#include <memory>

namespace frontend {
class Hud {
  public:
    Hud(core::World*);
    ~Hud();
    
    void Update(double delta_t);
    ugdk::ui::Node* node() { return node_.get(); }

  private:
    core::World* world_;

    std::shared_ptr<ugdk::ui::Node> node_, weapon_icon_;

    const skills::Skill *displayed_skill_;

    std::shared_ptr<ugdk::ui::Node> life_bar_, mana_bar_, block_bar_;

#ifdef HORUSEYE_DEBUG_TOOLS
    std::shared_ptr<ugdk::ui::Node> fps_meter_node_;
    double previous_fps_;
#endif
};

} // namespace frontend

#endif // HORUSEYE_FRONTEND_HUD_H_
