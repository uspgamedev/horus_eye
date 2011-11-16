
#ifndef HORUSEYE_GAME_RESOURCES_BLOCKRESOURCE_H_
#define HORUSEYE_GAME_RESOURCES_BLOCKRESOURCE_H_

#include <game/resources/containedresource.h>

namespace resource {

class CountableResource : public ContainedResource<int> {
  public:
    explicit CountableResource(int min_blocks = 0, int max_blocks = 0)
        : ContainedResource<int>(max_blocks, min_blocks, max_blocks) {}
};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_RESOURCES_BLOCKRESOURCE_H_ */
