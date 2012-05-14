#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/input/keys.h>
#include "playercontroller.h"

#include "game/components/hero.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"
#include "game/utils/constants.h"

using namespace ugdk;

using sprite::WorldObject;
using utils::Constants;

namespace component {

void PlayerController::Update(double dt) {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();

    Vector2D projectile_height(0, Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT);
    aim_destination_ = scene::World::FromScreenCoordinates(input_->GetMousePosition() + projectile_height);

    Direction d;
    if(input_->KeyDown(ugdk::input::K_w)) d |= Direction::Up();
    if(input_->KeyDown(ugdk::input::K_a)) d |= Direction::Left();
    if(input_->KeyDown(ugdk::input::K_s) && d.NumDirections() < 2) d |= Direction::Down();
    if(input_->KeyDown(ugdk::input::K_d) && d.NumDirections() < 2) d |= Direction::Right();
    dir_ = d;

    Hero* hero = static_cast<Hero*>(owner_->logic());
    if(hero->num_skills() > 0) {
        if (input_->KeyPressed(ugdk::input::K_e)) {
            int next_slot = skill_selected_;
            do next_slot = (next_slot+1) % Constants::HERO_MAX_WEAPONS;
            while (!hero->ChangeSecondaryWeapon(next_slot));
            skill_selected_ = next_slot;
        }
        if (input_->KeyPressed(ugdk::input::K_q)) {
            int next_slot = skill_selected_;
            do next_slot = ((next_slot-1) < 0) ? Constants::HERO_MAX_WEAPONS-1 : next_slot-1;
            while (!hero->ChangeSecondaryWeapon(next_slot));
            skill_selected_ = next_slot;
        }
    }

    current_direction_ = d.ToVector2D();
}

bool PlayerController::IsUsingSkillSlot(SkillSlot slot) const {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();
    switch(slot) {
    case PRIMARY:   return input_->MouseDown(ugdk::input::M_BUTTON_LEFT);
    case SECONDARY: return input_->MouseDown(ugdk::input::M_BUTTON_RIGHT);
    default: return false;
    }
}

}  // namespace sprite
