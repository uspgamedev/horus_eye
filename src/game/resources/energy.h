
#ifndef HORUSEYE_GAME_RESOURCES_ENERGY_H_
#define HORUSEYE_GAME_RESOURCES_ENERGY_H_

#include <game/resources/containedresource.h>

namespace resource {

class Energy : public ContainedResource<double> {

  public:

    typedef Resource<int> rate_t;

    explicit Energy(double max_value = 0.0, double variation_base = 0.0, int variation_rate = 1)
        : ContainedResource<double>(max_value, 0.0, max_value),
          variation_base_(variation_base),
          variation_rate_(variation_rate) {}

    void Update(double dt) { (*this) += variation_rate_.Get()*variation_base_*dt; }

    rate_t& variation_rate() { return variation_rate_; }

    void ChangeMaxValue(double value) {
        double proportion = Get()/max_value();
        set_max_value(value);
        Set(proportion*max_value());
    }

    void ChangeMinValue(double value) {
        double proportion = Get()/max_value();
        set_min_value(value);
        Set(proportion*max_value());
    }

  private:

    // TODO: find a way to make variation_base_ be const.
    double variation_base_;
    rate_t variation_rate_;

};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_RESOURCES_ENERGY_H_ */
