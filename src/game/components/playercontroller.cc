#include <algorithm>

#include "playercontroller.h"

#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/input/keys.h>

#include "game/components/logic/hero.h"
#include "game/components/caster.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"
#include "game/utils/constants.h"

using namespace ugdk;

using sprite::WorldObject;
using utils::Constants;

namespace component {
    
PlayerController::PlayerController(WorldObject* owner) 
    : super(owner) {
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
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();

    Vector2D projectile_height(0, Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT);
    aim_destination_ = scene::World::FromScreenCoordinates(input_->GetMousePosition() + projectile_height);

    Direction d;
    if(input_->KeyDown(ugdk::input::K_w)) d |= Direction::Up();
    if(input_->KeyDown(ugdk::input::K_a)) d |= Direction::Left();
    if(input_->KeyDown(ugdk::input::K_s) && d.NumDirections() < 2) d |= Direction::Down();
    if(input_->KeyDown(ugdk::input::K_d) && d.NumDirections() < 2) d |= Direction::Right();
    dir_ = d;

    if(known_skills_.size() > 0) {
        std::list<int>::const_iterator curr_it = selected_skill_;
        if (input_->KeyPressed(ugdk::input::K_e))
            cycle_iterator(selected_skill_, known_skills_, +1);
        if (input_->KeyPressed(ugdk::input::K_q))
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
    ugdk::input::InputManager *input_ = Engine::reference()->input_manager();
    switch(slot) {
    case PRIMARY:   return input_->MouseDown(ugdk::input::M_BUTTON_LEFT);
    case SECONDARY: return input_->MouseDown(ugdk::input::M_BUTTON_RIGHT);
    case SPECIAL1: return input_->KeyDown(ugdk::input::K_r);
    default: return false;
    }
}

}  // namespace sprite
