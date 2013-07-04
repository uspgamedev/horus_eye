#ifndef HORUS_EYE_GAME_BUILDERS_RECIPE_H_
#define HORUS_EYE_GAME_BUILDERS_RECIPE_H_

#include <vector>
#include <functional>
#include "game/sprites.h"

namespace builder {

typedef std::function<void (sprite::WorldObject*)> Property;

class Recipe {
  public:
    void AddProperty(const Property& property) {
        properties_.push_back(property);
    }
    sprite::WorldObject* Generate() const;

  private:
    std::vector<Property> properties_;
};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_RECIPE_H_ */
