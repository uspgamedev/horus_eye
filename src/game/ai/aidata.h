#ifndef HORUSEYE_GAME_AI_AIDATA_H_
#define HORUSEYE_GAME_AI_AIDATA_H_

#include <set>
#include <map>
#include <string>

#include "game/ai.h"
#include "game/components/controller.h"

namespace ai {

class AIData {
  public:

	AIData() {}
	~AIData();

    const ugdk::Vector2D& direction() const { return direction_; }
    void set_direction(const ugdk::Vector2D& dir) { direction_ = dir; }

    const ugdk::Vector2D& aim_destination() const { return aim_destination_; }
    void set_aim_destination(const ugdk::Vector2D& aim) { aim_destination_ = aim; }

    void AddUsingSkillSlot(component::Controller::SkillSlot slot) { using_skills_.insert(slot); }
    void RemoveUsingSkillSlot(component::Controller::SkillSlot slot) { using_skills_.erase(slot); }
    bool IsUsingSkillSlot(component::Controller::SkillSlot slot) { return using_skills_.count(slot); }

    void SetSharedData(const std::string& key, void* value);
    void* GetSharedData(const std::string& key);
    
    void Clear();

  protected:
    ugdk::Vector2D direction_;
    ugdk::Vector2D aim_destination_;
    typedef std::set< component::Controller::SkillSlot > SkillSlotSet;
    SkillSlotSet using_skills_;

    typedef std::map<std::string, void*> GenericDataMap;
    GenericDataMap shared_data_;
};

}
#endif // AI_DATA_H_
