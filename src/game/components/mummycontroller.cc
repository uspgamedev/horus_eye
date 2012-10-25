#include "mummycontroller.h"

#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/input/keys.h>
#include <ugdk/time/timeaccumulator.h>

#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/scenes/world.h"

using namespace ugdk;

using sprite::WorldObject;

#define EXP_PARAM (1.0)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {
    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);
}

namespace component {

// TODO: ignores standing

MummyController::MummyController(sprite::WorldObject* owner, double start_time_to_think) 
  : owner_(owner), standing_(true), start_time_to_think_(start_time_to_think), 
    time_to_think_(start_time_to_think), interval_(new ugdk::time::TimeAccumulator(0)) {}

MummyController::~MummyController() {
    delete interval_;
}

void MummyController::Think(double dt) {
    time_to_think_ -= dt;
    if(time_to_think_ <= 0){
        time_to_think_ = start_time_to_think_;

        if(WORLD()->hero())
            aim_destination_ = WORLD()->hero()->world_position();

        utils::VisionStrategy strategy;
        if(strategy.IsVisible(owner_, aim_destination_)) {
            standing_ = false;
            path_ = strategy.Calculate(owner_);
            if(!path_.empty()) updateDirection(path_.front());
        } else if(!standing_) {
            randomMovement();
        }
    }
}

void MummyController::randomMovement(){
    if (interval_->Expired()) {
        int dir = rand() % 8;

        Direction d;
        if (dir < 3) d |= Direction::Up();
        if (dir >= 2 && dir < 5) d |= Direction::Left();
        if (dir >= 4 && dir < 7) d |= Direction::Down();
        if (dir >= 6 || dir == 0) d |= Direction::Right();
        dir_ = d;

        interval_->Restart(WaitingTime());
        current_direction_ = d.ToVector2D();
        aim_destination_ = owner_->world_position() + 5*current_direction_;
    }
}

void MummyController::updateDirection(const Vector2D& destination){
    aim_destination_ = destination;
    current_direction_ = (destination - owner_->world_position()).Normalize();
    dir_ = Direction::FromWorldVector(current_direction_);
}

void MummyController::Update(double dt) {
    if(!owner_->is_active()) return;
    Think(dt);
}

bool MummyController::IsUsingSkillSlot(SkillSlot slot) const { return !standing_; }

}  // namespace sprite
