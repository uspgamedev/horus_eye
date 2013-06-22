#ifndef HORUS_EYE_GAME_BUILDERS_RECIPE_H_
#define HORUS_EYE_GAME_BUILDERS_RECIPE_H_

#include <vector>
#include <memory>
#include "game/sprites.h"

namespace builder {
     
class Director {
  public:
    virtual void Visit(sprite::WorldObject* wobj) = 0;
};

class Recipe {
  public:
    void AddDirector(const std::shared_ptr<Director>& director) {
        directors_.push_back(director);
    }
    sprite::WorldObject* Generate() const;

  private:
    std::vector<std::shared_ptr<Director>> directors_;
};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_RECIPE_H_ */
