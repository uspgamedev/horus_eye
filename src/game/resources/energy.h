
#ifndef HORUSEYE_GAME_RESOURCES_ENERGY_H_
#define HORUSEYE_GAME_RESOURCES_ENERGY_H_

#include <game/resources/containedresource.h>

namespace resource {

class Energy : public ContainedResource<float> {

  public:

    typedef Resource<int> rate_t;

    explicit Energy(float value = 0.0f, float variation_base = 0.0f, int variation_rate = 0.0f)
        : ContainedResource<float>(value, 0.0f, value),
          variation_base_(variation_base),
          variation_rate_(variation_rate) {}

    void Update(float dt) { (*this) += variation_rate_.Get()*variation_base_*dt; }

    rate_t& variation_rate() { return variation_rate_; }

  private:

    // TODO: find a way to make variation_base_ be const.
    float variation_base_;
    rate_t variation_rate_;

};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_RESOURCES_ENERGY_H_ */
