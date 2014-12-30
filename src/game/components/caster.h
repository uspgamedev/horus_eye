#ifndef HORUSEYE_COMPONENT_CASTER_H_
#define HORUSEYE_COMPONENT_CASTER_H_

#include "game/components/base.h"

#include "game/sprites.h"
#include "game/skills.h"

#include "game/components/controller.h"
#include "game/components/orders.h"
#include "game/resources/resource.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/skills/usearguments.h"

#include <ugdk/system/compatibility.h>
#include <ugdk/util/idgenerator.h>

#include <map>
#include <vector>
#include <cstdlib>

namespace component {

class Caster : public Base {
  public:
    static const char* DEFAULT_NAME() { return "caster"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    Caster(const resource::Energy& mana, int block_count, const skills::usearguments::Aim& aim);

    /// Easy constructor. Default block_count and aim.
    /**
     * Default block_count: 1
     * Default aim: from owner position, to owner's controller aim_destination
     */
    Caster(const sprite::ObjectHandle& handle, const resource::Energy& mana);

    ~Caster();

    // METHODS

    void Update(double dt) override;
    void OnAdd(sprite::WObjRawPtr) override;

    bool CastSkill(Controller::SkillSlot slot);

    const skills::Skill* SkillAt(Controller::SkillSlot slot) const;

    int LearnSkill(const skills::Skill* skill);
    int LearnSkill(const std::string& skill_name);

    void UnlearnSkill(int id);

    /// Invalid id means unequip given slot.
    void EquipSkill(int id, Controller::SkillSlot);

    int LearnAndEquipSkill(const std::string& skill_name, Controller::SkillSlot slot) {
        int id = LearnSkill(skill_name);
        EquipSkill(id, slot);
        return id;
    }

    /// Returns your maximum mana when with all mana blocks.
    double FullMana() { return mana_blocks_.TotalCapcity(); }

    // GETTERS
    sprite::WObjRawPtr owner() const { return owner_; }

          resource::Energy& mana()       { return mana_; }
    const resource::Energy& mana() const { return mana_; }

          resource::CapacityBlocks& mana_blocks()       { return mana_blocks_; }
    const resource::CapacityBlocks& mana_blocks() const { return mana_blocks_; }
    
    const skills::usearguments::Aim& aim() const { return aim_; }

    size_t num_skills() const { return skills_.size(); }

          resource::Resource<int>& power()       { return power_; }
    const resource::Resource<int>& power() const { return power_; }

    /// Returns the current maximum mana.
    double max_mana() const { return mana_.max_value(); }

    // SETTERS
    void set_mana(resource::Energy &mana) { mana_ = mana; }
    void set_mana(double mana) { mana_.Set(mana); }

  protected:
    void unequipSkill(const skills::Skill* skill);

  private:
    /// The owner.
    sprite::WObjRawPtr owner_;

    /// The known skills.
    std::vector<const skills::Skill*> skills_;

    /// The active skills this caster has.
    std::map<Controller::SkillSlot, const skills::Skill*> active_skills_;

    /// The mana of this caster. An energy manages reneration.
    resource::Energy mana_;

    /// Mana is separated in different blocks.
    resource::CapacityBlocks mana_blocks_;

    /// Where this caster is aiming.
    skills::usearguments::Aim aim_;

    /// TODO
    ugdk::util::IDGenerator skill_id_generator_;

    ///
    resource::Resource<int> power_;

};  // class Caster

}  // namespace component

#endif  // HORUSEYE_COMPONENT_CASTER_H_
