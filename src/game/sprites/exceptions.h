#ifndef HORUSEYE_GAME_SPRITES_EXCEPTIONS_H_
#define HORUSEYE_GAME_SPRITES_EXCEPTIONS_H_

#include <ugdk/system/exceptions.h>

namespace sprite {

class InvalidObject : public ugdk::system::BaseException {
  public:
    InvalidObject() : ugdk::system::BaseException("Attempting to operate on unattached ObjectHandle.") {}
};

} // namespace sprite

#endif // HORUSEYE_GAME_SPRITES_EXCEPTIONS_H_