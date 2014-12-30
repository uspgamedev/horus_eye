#ifndef HORUSEYE_GAME_SPRITE_OBJECTHANDLE_H_
#define HORUSEYE_GAME_SPRITE_OBJECTHANDLE_H_

#include "game/sprites.h"

#include <memory>

namespace sprite {

class ObjectHandle {
  public:
    ObjectHandle();
    ObjectHandle(const ObjectHandle&);
    ObjectHandle(const WObjPtr& r);
    ObjectHandle(const WObjWeakPtr& r);

    ObjectHandle operator=(const WObjPtr& r);

    bool attached() const;

    WorldObject* operator-> () const;
  private:
    WObjWeakPtr pointer_;

};  // class ObjectHandle

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_OBJECTHANDLE_H_
