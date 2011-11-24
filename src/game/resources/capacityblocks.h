
#ifndef HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_
#define HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_

#include <game/resources/countableresource.h>
#include <game/resources/energy.h>


namespace resource {

class CapacityBlocks : public CountableResource {

  public:

    explicit CapacityBlocks(Energy& contents, int total_blocks = 1, float capacity_per_block = 1.0f)
        : CountableResource(1, total_blocks),
          contents_(contents),
          capacity_per_block_(capacity_per_block) {}

    Energy& contents() { return contents_; }

    void Set(int quantity, int filled) {
        CountableResource::Set(quantity);
        contents_.set_max_value(ToMana(quantity));
        contents_.Set(ToMana(filled));
    }

    void Set(int quantity) {
        Set(quantity, quantity);
    }

    void Decrease(int quantity, int filled) {
        CountableResource::Decrease(quantity);
        contents_ -= ToMana(filled);
        contents_.set_max_value(ToMana(Get()));
    }

    void Decrease(int quantity) {
        Decrease(quantity, quantity);
    }

    void Increase(int quantity, int filled) {
        CountableResource::Increase(quantity);
        contents_ += ToMana(filled);
        contents_.set_max_value(ToMana(Get()));
    }

    void Increase(int quantity) {
        Increase(quantity, quantity);
    }

    void Fill() {
        Set(max_value());
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
