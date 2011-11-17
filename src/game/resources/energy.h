
#ifndef HORUSEYE_GAME_RESOURCES_SIMPLE_RESOURCE_H_
#define HORUSEYE_GAME_RESOURCES_SIMPLE_RESOURCE_H_

#include <game/resources/containedresource.h>

namespace resource {

class Energy : public ContainedResource<float> {
  public:

    explicit Energy(float value = 0.0f, float variation_rate = 0.0f)
        : ContainedResource<float>(value, 0.0f, value), variation_rate_(variation_rate) {}

    void Update(float dt) { (*this) += variation_rate_*dt; }

  private:

    float variation_rate_;

};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_RESOURCES_SIMPLE_RESOURCE_H_ */
