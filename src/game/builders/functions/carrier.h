#ifndef HORUSEYE_GAME_BUILDERS_FUNCTIONS_CARRIER_H_
#define HORUSEYE_GAME_BUILDERS_FUNCTIONS_CARRIER_H_

#include <list>
#include "game/sprites.h"

namespace builder {
namespace function {

class Carrier {
  protected:
    std::list<sprite::WorldObject*> drop_list_;
  public:
    Carrier(std::list<sprite::WorldObject*> &list) : drop_list_(list) {}
    Carrier(sprite::WorldObject *drop) { drop_list_.push_back(drop); }

    void operator()(sprite::WorldObject *wobj);
};

} // namespace function
} // namespace builder

#endif // HORUSEYE_GAME_BUILDERS_FUNCTIONS_CARRIER_H_
