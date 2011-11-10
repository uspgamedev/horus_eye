
#ifndef HORUSEYE_GAME_SPRITES_RESOURCES_LIFE_H_
#define HORUSEYE_GAME_SPRITES_RESOURCES_LIFE_H_

#include <game/resources/containedresource.h>

namespace resource {

class Life : public ContainedResource<float> {
  public:
    explicit Life(float max_life = 0.0f) : ContainedResource<float>(max_life, 0.0f, max_life) {}
};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_SPRITES_RESOURCES_LIFE_H_ */
