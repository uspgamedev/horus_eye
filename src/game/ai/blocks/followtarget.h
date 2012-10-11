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
	FollowTarget() : target_(0), target_tag_(""), search_target_(false) {}
	~FollowTarget() {}

	virtual void Start();
	virtual AIModule::Status Update(double dt, AIData* data);
	virtual void Finish();

    sprite::WorldObject* target() const { return target_; }
    void set_target(sprite::WorldObject* target) { target_ = target; }
    const std::string& target_tag() const { return target_tag_; }
    void set_target_tag(const std::string& tag) { target_tag_ = tag; }

protected:
	utils::VisionStrategy vision_strategy_;
    
    sprite::WorldObject* target_;
    std::string target_tag_;
    bool search_target_;
    ugdk::Vector2D last_known_target_pos_;
};

}
}
#endif // CHECKTARGETVISIBLE_H_
