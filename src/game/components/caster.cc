#include "game/components/caster.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>

#include "game/sprites/worldobject.h"
#include "game/sprites/objecthandle.h"
#include "game/skills/skill.h"
#include "game/components/animation.h"

using skills::Skill;

const static int ID_GENERATOR_INVALID_ID = -1;
const static int MAX_ID = 16;

namespace component {

Caster::Caster(const resource::Energy& mana, int block_count, const skills::usearguments::Aim& aim)
    :   owner_(nullptr)
    ,   mana_(mana)
    ,   mana_blocks_(mana_, block_count)
    ,   aim_(aim)
    ,   skill_id_generator_(0, MAX_ID, ID_GENERATOR_INVALID_ID)
    ,   power_(100)
{}

Caster::Caster(const sprite::ObjectHandle& handle, const resource::Energy& mana)
    :   owner_(nullptr)
    ,   mana_(mana)
    ,   mana_blocks_(mana_, 1)
    ,   aim_(handle->world_position(), handle->controller()->aim_destination())
    ,   skill_id_generator_(0, MAX_ID, ID_GENERATOR_INVALID_ID)
    ,   power_(100)
{}

Caster::~Caster() {}

// ============= other stuff

void Caster::Update(double dt) {
    mana_.Update(dt);
    if(!owner_->component<Animation>()->CanInterrupt(utils::ATTACK)) return;
    
    Controller* controller = owner_->controller();
    if(!controller) return;
    for(const auto& slot : active_skills_) {
        if(slot.second && controller->IsUsingSkillSlot(slot.first) && slot.second->IsValidUse(this)) {
            if(this->CastSkill(slot.first)) {
                owner_->component<Animation>()->ChangeAnimation(utils::ATTACK, 
                    Direction::FromWorldVector(aim_.destination_ - aim_.origin_));
                break;
            }
        }
    }
}
    
void Caster::OnAdd(sprite::WObjRawPtr owner) {
    owner_ = owner;
}

bool Caster::CastSkill(Controller::SkillSlot slot) {
    const Skill* skill = active_skills_[slot];
    if(skill && skill->Available(this)) {
        skill->Use(this);
        return true;
    }
    return false;
}

const skills::Skill* Caster::SkillAt(Controller::SkillSlot slot) const {
    auto it = active_skills_.find(slot);
    return (it != active_skills_.end()) ? it->second : nullptr;
}

int Caster::LearnSkill(const skills::Skill* skill) {
    if(!skill) return ID_GENERATOR_INVALID_ID;
    int id = skill_id_generator_.GenerateID();
    if(id != ID_GENERATOR_INVALID_ID) {
        if(static_cast<size_t>(id) + 1 >= skills_.size())
            skills_.resize(static_cast<size_t>(id) + 1);
        skills_[id] = skill;
    }
    return id;
}

int Caster::LearnSkill(const std::string& skill_name) {
    return LearnSkill(ugdk::resource::manager()->get_container<skills::Skill*>()->Find(skill_name));
}

void Caster::UnlearnSkill(int id) {
    int err = skill_id_generator_.ReleaseID(id);
    if(err != ID_GENERATOR_INVALID_ID) {
        unequipSkill(skills_[id]);
        skills_[id] = nullptr;
    }
}

void Caster::EquipSkill(int id, Controller::SkillSlot skill_slot) {
    if(id >= 0 && static_cast<size_t>(id) < skills_.size())
        active_skills_[skill_slot] = skills_[id];
}
    
void Caster::unequipSkill(const skills::Skill* skill) {
    for(auto& slot : active_skills_)
        if(slot.second == skill)
            slot.second = nullptr;
}

}  // namespace sprite
