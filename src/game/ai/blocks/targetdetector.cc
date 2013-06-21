#include "game/ai/blocks/targetdetector.h"
#include "game/ai/aidata.h"
#include "game/ai/ai.h"
#include "game/sprites/worldobject.h"
#include "game/context.h"
#include "game/components/animation.h"
#include <pyramidworks/geometry/geometricshape.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/portable/tr1.h>
#include <functional>
#include <algorithm>
#include <string>
#include <cstdio>

namespace ai {
namespace blocks {

using ugdk::math::Vector2D;
using sprite::WorldObject;
using std::function;
using std::bind;
using namespace std::placeholders;

TargetDetector::~TargetDetector() {
    delete area_;
}

void TargetDetector::Start() {
}

static bool CompareWObjDist(WorldObject* owner, WorldObject* first, WorldObject* second) {
    double dist1 = (owner->world_position() - first->world_position()).LengthSquared();
    double dist2 = (owner->world_position() - second->world_position()).LengthSquared();
    return dist1 < dist2;
}

AIModule::Status TargetDetector::Update(double dt, AIData* data) {
	WorldObject* owner = parent_->base()->owner();
	
    if (area_ == NULL || target_classname_.size() <= 0 || identifier_.size() <= 0) {
        return AIModule::DORMANT;
    }

    possible_targets_.clear();
    context::GetCollidingObjects(target_classname_, *area_, owner->world_position(), possible_targets_);
    function<bool (WorldObject*, WorldObject*)> comp = bind(CompareWObjDist, owner, _1, _2);
    //possible_targets_.sort(comp);
    std::sort(possible_targets_.begin(), possible_targets_.end(), comp);

    WorldObject* closest_target = target();
    if (closest_target) {
        ugdk::script::VirtualObj vtarget (data->script_wrapper());
        vtarget.set_value<WorldObject*>(closest_target);
        vtarget.value<WorldObject*>(true); /*POG: set_value is giving ownership to the script... IT MUST NOT. */
        data->SetSharedData(identifier_+"_target", vtarget);
    }
    else {
        data->ClearSharedData(identifier_+"_target");
    }

	return AIModule::ACTIVE;
}

void TargetDetector::Finish() {
}

void TargetDetector::set_area(pyramidworks::geometry::GeometricShape* area) {
    if (area_)  delete area_;
    area_ = area;
}

sprite::WorldObject* TargetDetector::target() {
    if (possible_targets_.empty())  return NULL;
    return possible_targets_.front();
}

}
}
