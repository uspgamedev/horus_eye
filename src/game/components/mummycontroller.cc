#include <ugdk/base/engine.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/input/keys.h>
#include <ugdk/time/timeaccumulator.h>
#include "mummycontroller.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"

using namespace ugdk;

using sprite::WorldObject;

#define TIME_TO_THINK 0.1
#define EXP_PARAM (1.0)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {
    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);
}

namespace component {

// TODO: ignores standing

MummyController::MummyController(sprite::WorldObject* owner) 
    : super(owner),
    interval_(new ugdk::time::TimeAccumulator(0)) {}

MummyController::~MummyController() {
    delete interval_;
}

void MummyController::Think(double dt) {
    time_to_think_ -= dt;
    if(time_to_think_ <= 0){
        time_to_think_ = TIME_TO_THINK;
        utils::VisionStrategy strategy;
        if(strategy.IsVisible(owner_->world_position()))
            path_ = strategy.Calculate(owner_->world_position());
        else 
            RandomMovement();
    }
}

void MummyController::RandomMovement(){
    double PI = acos(-1.0);

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
    }
}

void MummyController::Update(double dt) {
    if(!owner_->is_active()) return;
    Think(dt);
}

bool MummyController::IsUsingSkillSlot(SkillSlot slot) const { return true; }

}  // namespace sprite
