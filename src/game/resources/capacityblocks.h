
#ifndef HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_
#define HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_

#include <game/resources/countableresource.h>
#include <game/resources/energy.h>


namespace resource {

class CapacityBlocks : public CountableResource {

  public:

    explicit CapacityBlocks(Energy& contents, int total_blocks = 1)
        : CountableResource(1, total_blocks),
          contents_(contents),
          capacity_per_block_(contents.max_value() / total_blocks) {}

          Energy& contents()       { return contents_; }
    const Energy& contents() const { return contents_; }

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
    double ToMana(int blocks) const {
        return blocks*capacity_per_block_;
    }

    double TotalCapcity() const {
    	return max_value() * capacity_per_block_;
    }

  private:

    Energy& contents_;
    const double capacity_per_block_;

    CapacityBlocks& operator=(CapacityBlocks&);

};

} /* namespace sprite */

#endif /* HORUSEYE_GAME_RESOURCES_CAPACITYBLOCKS_H_ */
