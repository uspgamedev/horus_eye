
#ifndef HORUSEYE_GAME_RESOURCES_SIMPLE_RESOURCE_H_
#define HORUSEYE_GAME_RESOURCES_SIMPLE_RESOURCE_H_

#include <game/resources/containedresource.h>

namespace resource {

class SimpleResource : public ContainedResource<float> {
  public:
    explicit SimpleResource(float value = 0.0f) : ContainedResource<float>(value, 0.0f, value) {}
};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_RESOURCES_SIMPLE_RESOURCE_H_ */
