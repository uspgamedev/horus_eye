#include <algorithm>

#include "playercontroller.h"

#include <ugdk/system/engine.h>
#include <ugdk/input/module.h>

#include "game/components/caster.h"
#include "game/sprites/worldobject.h"
#include "game/core/coordinates.h"
#include "game/constants.h"

using namespace ugdk;

using ugdk::input::Scancode;
using ugdk::input::Keycode;
using ugdk::input::MouseButton;
using sprite::WorldObject;

namespace component {
    
PlayerController::PlayerController(WorldObject* owner) 
    :   owner_(owner),
        mouse_aim_offset_(0.0, constants::GetDouble("PLAYER_MOUSE_HEIGHT_OFFSET")) {
    selected_skill_ = known_skills_.begin();
}

static void cycle_iterator(std::list<int>::const_iterator& it, const std::list<int>& range, int dir) {
    if(dir > 0) {
        ++it;
        if(it == range.end())
            it = range.begin();
    } else {
        if(it == range.begin())
            it = range.end();
        --it;
    }
}

void PlayerController::Update(double dt) {
    const auto& keyboard = ugdk::input::manager()->keyboard();
    const auto& mouse = ugdk::input::manager()->mouse();

    aim_destination_ = core::FromScreenCoordinates(mouse.position() + mouse_aim_offset_);

    Direction d;
    if(keyboard.IsDown(Scancode::W)) d |= Direction::Up();
    if(keyboard.IsDown(Scancode::A)) d |= Direction::Left();
    if(keyboard.IsDown(Scancode::S) && d.NumDirections() < 2) d |= Direction::Down();
    if(keyboard.IsDown(Scancode::D) && d.NumDirections() < 2) d |= Direction::Right();
    dir_ = d;

    if(known_skills_.size() > 0) {
        std::list<int>::const_iterator curr_it = selected_skill_;
        if(keyboard.IsPressed(Scancode::E))
            cycle_iterator(selected_skill_, known_skills_, +1);
        if(keyboard.IsPressed(Scancode::Q))
            cycle_iterator(selected_skill_, known_skills_, -1);

        if(selected_skill_ != curr_it)
            owner_->caster()->EquipSkill(*selected_skill_, Controller::SECONDARY);
    }

    current_direction_ = d.ToVector2D();
}

void PlayerController::AddSkill(int id) {
    known_skills_.push_back(id);
    if(selected_skill_ == known_skills_.end())
        selected_skill_ = known_skills_.begin();
}

void PlayerController::RemoveSkill(int id) {
    while(selected_skill_ != known_skills_.end() && id == *selected_skill_) 
        ++selected_skill_;
    known_skills_.remove(id);
    if(selected_skill_ == known_skills_.end())
        selected_skill_ = known_skills_.begin();
}

bool PlayerController::IsUsingSkillSlot(SkillSlot slot) const {
    const auto& mouse = ugdk::input::manager()->mouse();
    switch(slot) {
    case PRIMARY:   return mouse.IsDown(MouseButton::LEFT);
    case SECONDARY: return mouse.IsDown(MouseButton::RIGHT);
    case SPECIAL1: return ugdk::input::manager()->keyboard().IsDown(Scancode::R);
    default: return false;
    }
}

}  // namespace sprite
