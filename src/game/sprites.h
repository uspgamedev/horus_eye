#ifndef HORUSEYE_GAME_SPRITES_H_
#define HORUSEYE_GAME_SPRITES_H_

#include <memory>

namespace sprite {

class Effect;
class ItemEvent;
class WorldObject;

typedef std::shared_ptr<WorldObject> WObjPtr;
typedef std::weak_ptr<WorldObject> WObjWeakPtr;

}  // namespace sprite

#endif // HORUSEYE_GAME_SPRITES_H_
