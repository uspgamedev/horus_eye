#ifndef HORUSEYE_GAME_AI_BLOCK_CHECKTARGETVISIBLE_H_
#define HORUSEYE_GAME_AI_BLOCK_CHECKTARGETVISIBLE_H_

#include <string>
#include "game/sprites.h"
#include "game/ai.h"
#include "game/ai/logicblock.h"
#include "game/utils/visionstrategy.h"


namespace ai {
namespace blocks {

class FollowTarget : public LogicBlock {

public:
	FollowTarget() : detector_identifier_(""), search_target_(false) {}
	~FollowTarget() {}

	virtual void Start();
	virtual AIModule::Status Update(double dt, AIData* data);
	virtual void Finish();

    const std::string& detector_identifier() const { return detector_identifier_; }
    void set_detector_identifier(const std::string& identifier) { detector_identifier_ = identifier; }

protected:
	utils::VisionStrategy vision_strategy_;
    
    std::string detector_identifier_;
    bool search_target_;
    ugdk::math::Vector2D last_known_target_pos_;
};

}
}
#endif // CHECKTARGETVISIBLE_H_
