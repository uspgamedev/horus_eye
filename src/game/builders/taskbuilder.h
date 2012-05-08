#ifndef HORUSEYE_GAME_BUILDERS_TASKBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_TASKBUILDER_H_

#include <ugdk/action.h>
#include <game/utils/tilefwd.h>

namespace sprite {
    class WorldObject;
}

namespace builder {

class TaskBuilder {
  public:
    TaskBuilder() {}
    ~TaskBuilder() {}

    ugdk::action::Task* PauseMenuTask() const;
    ugdk::action::Task* VisibilityTask(sprite::WorldObject* hero, utils::GameMap& map) const;
};

}

#endif /* HORUSEYE_GAME_BUILDERS_TASKBUILDER_H_ */
