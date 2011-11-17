#ifndef HORUSEYE_GAME_SKILLS_CASTARGUMENTS_H_
#define HORUSEYE_GAME_SKILLS_CASTARGUMENTS_H_

#include <ugdk/math/vector2D.h>

namespace skills {
namespace castarguments {

/**
 *  Position where to cast single-positioned skills.
 *  For example, summons, spells cast directly at the cursor's position, etc...
 */
typedef ugdk::Vector2D Position;

/**
 *  Aim for aimed skills. Specially useful for projectile skills.
 *
 *  origin == projectile's starting position, usually is the caster's position + offset.
 *  destination == where the projectile is aimed at. Usually it's the cursor's position
 *      for player skills, or the Hero's current location + rand for AI skills.
 */
class Aim {
  public:
    Aim(Position& origin, Position& destination)
        : origin_(origin), destination_(destination) {}

    Position& origin_;
    Position& destination_;
};

} // castarguments
} // skills

#endif
