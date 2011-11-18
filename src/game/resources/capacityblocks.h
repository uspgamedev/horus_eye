
#ifndef HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_
#define HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_

#include <game/resources/countableresource.h>
#include <game/resources/energy.h>


namespace resource {

class CapacityBlocks : public CountableResource {

  public:

    explicit CapacityBlocks(Energy& contents, float value = 1, float capacity_per_block = 0.0f)
        : CountableResource(value, value),
          contents_(contents),
          capacity_per_block_(capacity_per_block) {}

    Energy& contents() { return contents_; }

    void Decrease(int quantity) {
        CountableResource::Decrease(quantity);
        contents_.set_max_value(Get()*capacity_per_block_);
    }

    void Increase(int quantity) {
        CountableResource::Increase(quantity);
        contents_.set_max_value(Get()*capacity_per_block_);
    }

    /// Uses this objects capacity to convert a number of blocks into the amount of energy they
    /// contain.
    /**
     * TODO
     */
    float ToMana(int blocks) {
        return blocks*capacity_per_block_;
    }

  private:

    Energy& contents_;
    const float capacity_per_block_;

};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_ */
