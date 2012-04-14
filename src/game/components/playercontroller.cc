#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/input/keys.h>
#include "playercontroller.h"
#include "game/sprites/worldobject.h"

using namespace ugdk;

using sprite::WorldObject;

namespace component {

void PlayerController::Update(double dt) {
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();
    bool pressed_key[4];

    for (int i = 0; i < 4; i++) {
        pressed_key[i] = false;
    }

    Direction d;
    if(input_->KeyDown(ugdk::input::K_w)) d |= Direction::Up();
    if(input_->KeyDown(ugdk::input::K_a)) d |= Direction::Left();
    if(input_->KeyDown(ugdk::input::K_s) && d.NumDirections() < 2) d |= Direction::Down();
    if(input_->KeyDown(ugdk::input::K_d) && d.NumDirections() < 2) d |= Direction::Right();

    dir_ = d;

    /* No weapons for u.
    if(weapons_.size() > 0) {
        if (input_->KeyPressed(ugdk::input::K_e)) {
            int next_slot = slot_selected_;
            do next_slot = (next_slot+1)%Constants::HERO_MAX_WEAPONS;
            while (!weapons_.count(next_slot));
            ChangeSecondaryWeapon(next_slot);
        }
        if (input_->KeyPressed(ugdk::input::K_q)) {
            int next_slot = slot_selected_;
            do next_slot = next_slot-1 < 0 ? Constants::HERO_MAX_WEAPONS-1 : next_slot-1;
            while (!weapons_.count(next_slot));
            ChangeSecondaryWeapon(next_slot);
        }
    } */

    /* Reference code.
    directions_[Direction_::RIGHT] = Vector2D(1, -1);
    directions_[Direction_::LEFT] = Vector2D(-1, 1);
    directions_[Direction_::DOWN] =  Vector2D(-1, -1);
    directions_[Direction_::UP] = Vector2D(1, 1);
    */
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
