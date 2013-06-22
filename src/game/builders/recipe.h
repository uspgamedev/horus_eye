#ifndef HORUS_EYE_GAME_BUILDERS_RECIPE_H_
#define HORUS_EYE_GAME_BUILDERS_RECIPE_H_

#include <vector>
#include <memory>
#include <functional>
#include "game/sprites.h"

namespace builder {

typedef std::function<void (sprite::WorldObject*)> Director;

class Recipe {
  public:
    void AddDirector(const Director& director) {
        directors_.push_back(director);
    }
    sprite::WorldObject* Generate() const;

  private:
    std::vector<Director> directors_;
};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_RECIPE_H_ */
