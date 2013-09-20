#ifndef HORUSEYE_GAME_BUILDERS_FUNCTIONS_CARRIER_H_
#define HORUSEYE_GAME_BUILDERS_FUNCTIONS_CARRIER_H_

#include <list>
#include "game/sprites.h"

namespace builder {
namespace function {

class Carrier {
  protected:
    std::list<sprite::WObjPtr> drop_list_;

  public:
    Carrier(std::list<sprite::WObjPtr>& list) : drop_list_(list) {}
    Carrier(const sprite::WObjPtr& drop) : drop_list_(1, drop) {}

    void operator()(sprite::WorldObject *wobj);
};

} // namespace function
} // namespace builder

#endif // HORUSEYE_GAME_BUILDERS_FUNCTIONS_CARRIER_H_
