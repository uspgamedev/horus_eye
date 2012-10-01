#ifndef HORUSEYE_GAME_AI_AIDATA_H_
#define HORUSEYE_GAME_AI_AIDATA_H_

#include "game/ai.h"
#include "game/components/controller.h"
#include <set>

namespace ai {

class AIData {
  public:

	AIData() {}
	~AIData() {}

    const ugdk::Vector2D& direction() const { return direction_; }
    void set_direction(ugdk::Vector2D& dir) { direction_ = dir; }

    const ugdk::Vector2D& aim_destination() const { return aim_destination_; }
    void set_aim_destination(ugdk::Vector2D& aim) { aim_destination_ = aim; }

    void AddUsingSkillSlot(component::Controller::SkillSlot slot) { using_skills_.insert(slot); }
    void RemoveUsingSkillSlot(component::Controller::SkillSlot slot) { using_skills_.erase(slot); }
    bool IsUsingSkillSlot(component::Controller::SkillSlot slot) { return using_skills_.count(slot); }
    
    void Clear() {
        direction_ = ugdk::Vector2D();
        aim_destination_ = ugdk::Vector2D();
        using_skills_.clear();
    }

  protected:
    ugdk::Vector2D direction_;
    ugdk::Vector2D aim_destination_;
    typedef std::set< component::Controller::SkillSlot > SkillSlotSet;
    SkillSlotSet using_skills_;
};

}
#endif // AI_DATA_H_
