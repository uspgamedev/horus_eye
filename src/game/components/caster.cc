#include "game/components/caster.h"

#include "game/sprites/worldobject.h"
#include "game/skills/skill.h"

using sprite::WorldObject;
using skills::Skill;

#define PI 3.1415926535897932384626433832795

#define ID_GENERATOR_INVALID_ID -1
#define MAX_ID 16

namespace component {

Caster::Caster(WorldObject* owner, const resource::Energy& mana, int block_count, const skills::usearguments::Aim& aim)
    : owner_(owner), mana_(mana), mana_blocks_(mana_, block_count),
      aim_(aim), skill_id_generator_(0, MAX_ID, ID_GENERATOR_INVALID_ID) {}

Caster::Caster(sprite::WorldObject* owner, const resource::Energy& mana)
    : owner_(owner), mana_(mana), mana_blocks_(mana_, 1),
      aim_(owner->world_position(), owner->controller()->aim_destination()), 
      skill_id_generator_(0, MAX_ID, ID_GENERATOR_INVALID_ID) {}

Caster::~Caster() {
    std::vector<skills::Skill*>::iterator it;
    for(it = skills_.begin(); it != skills_.end(); ++it)
        delete *it;
}

// ============= other stuff

void Caster::Update(double dt) {
    mana_.Update(dt);
}

bool Caster::CastSkill(Controller::SkillSlot slot) {
    Skill* skill = active_skills_[slot];
    if(skill && skill->Available()) {
        skill->Use();
        return true;
    }
    return false;
}

skills::Skill* Caster::SkillAt(Controller::SkillSlot slot) {
    return active_skills_[slot];
}

int Caster::LearnSkill(skills::Skill* skill) {
    int id = skill_id_generator_.GenerateID();
    if(id != ID_GENERATOR_INVALID_ID) {
        if(static_cast<size_t>(id) + 1 >= skills_.size())
            skills_.resize(static_cast<size_t>(id) + 1);
        skills_[id] = skill;
    }
    return id;
}

void Caster::UnlearnSkill(int id) {
    int err = skill_id_generator_.ReleaseID(id);
    if(err != ID_GENERATOR_INVALID_ID) {
        unequipSkill(skills_[id]);
        delete skills_[id];
        skills_[id] = NULL;
    }
}

void Caster::EquipSkill(int id, Controller::SkillSlot skill_slot) {
    if(id >= 0 && static_cast<size_t>(id) < skills_.size())
        active_skills_[skill_slot] = skills_[id];
}
    
void Caster::unequipSkill(skills::Skill* skill) {
    std::map<Controller::SkillSlot, skills::Skill*>::iterator sk;
    for(sk = active_skills_.begin(); sk != active_skills_.end(); ++sk) {
        if(sk->second == skill)
            sk->second = NULL;
    }
}

}  // namespace sprite
