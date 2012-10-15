#ifndef HORUSEYE_GAME_AI_BLOCK_TARGETDETECTOR_H_
#define HORUSEYE_GAME_AI_BLOCK_TARGETDETECTOR_H_

#include <string>
#include <vector>
#include <pyramidworks/geometry.h>
#include "game/sprites.h"
#include "game/ai.h"
#include "game/ai/logicblock.h"


namespace ai {
namespace blocks {

class TargetDetector : public LogicBlock {

public:
	TargetDetector() : target_classname_(""), area_(0) {}
	~TargetDetector();

	virtual void Start();
	virtual AIModule::Status Update(double dt, AIData* data);
	virtual void Finish();

    const std::string& target_classname() const { return target_classname_; }
    void set_target_classname(const std::string& target_classname) { target_classname_ = target_classname; }

    pyramidworks::geometry::GeometricShape* area() const { return area_; }
    void set_area(pyramidworks::geometry::GeometricShape* area);

    const std::string& identifier() const { return identifier_; }
    void set_identifier(const std::string& identifier) { identifier_ = identifier; }

    sprite::WorldObject* target();
    const std::vector<sprite::WorldObject*>& possible_targets() { return possible_targets_; }

protected:
    std::string target_classname_;
    pyramidworks::geometry::GeometricShape* area_;
    std::string identifier_;

    std::vector<sprite::WorldObject*> possible_targets_;
};

}
}
#endif // TARGETDETECTOR_H_
