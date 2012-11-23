#ifndef HORUSEYE_GAME_BUILDERS_TASKBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_TASKBUILDER_H_

#include <ugdk/action.h>

namespace builder {

class TaskBuilder {
  public:
    TaskBuilder() {}
    ~TaskBuilder() {}

    ugdk::action::Task* PauseMenuTask() const;
};

}

#endif /* HORUSEYE_GAME_BUILDERS_TASKBUILDER_H_ */
