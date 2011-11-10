
#ifndef HORUSEYE_GAME_SPRITES_RESOURCES_CONTAINEDRESOURCE_H_
#define HORUSEYE_GAME_SPRITES_RESOURCES_CONTAINEDRESOURCE_H_

#include <game/resources/resource.h>

#include <algorithm>

namespace resource {

/// TODO document.
template <class resource_t>
class ContainedResource : public Resource<resource_t> {

  public:

    explicit ContainedResource(resource_t base_value, resource_t min_value, resource_t max_value)
        : Resource<resource_t>(base_value), min_value_(min_value), max_value_(max_value) {}

    ~ContainedResource() {}

    void Set(resource_t quantity) {
        super::Set(quantity);
        Normalize();
    }

    void Increase(resource_t quantity) {
        super::Increase(quantity);
        Normalize();
    }

    void Decrease(resource_t quantity) {
        super::Decrease(quantity);
        Normalize();
    }

    bool Empty() {
        return super::resource_pool() <= min_value_;
    }

    bool Full() {
        return max_value_ <= super::resource_pool();
    }

    void Flush() {
        super::Set(min_value_);
    }

    void Fill() {
        super::Set(max_value_);
    }

    resource_t min_value() const {
        return min_value_;
    }
    void set_min_value(resource_t min_value) {
        min_value_ = min_value;
        Normalize();
    }

    resource_t max_value() const {
        return max_value_;
    }
    void set_max_value(resource_t max_value) {
        max_value_ = max_value;
        Normalize();
    }

  protected:

    void Normalize() {
        super::resource_pool() = std::min(super::resource_pool(), max_value_);
        super::resource_pool() = std::max(super::resource_pool(), min_value_);
    }

  private:

    typedef Resource<resource_t> super;

    resource_t min_value_, max_value_;

};

} /* namespace resource */
#endif /* HORUSEYE_GAME_SPRITES_RESOURCES_CONTAINEDRESOURCE_H_ */
