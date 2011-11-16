#ifndef HORUSEYE_GAME_SKILLS_CASTARGUMENTS_H_
#define HORUSEYE_GAME_SKILLS_CASTARGUMENTS_H_

#include <ugdk/math/vector2D.h>

namespace skills {
namespace castarguments {

/**
 *  Aim for aimed skills. Specially useful for projectile skills.
 *
 *  origin == projectile's starting position, usually is the caster's position + offset.
 *  destination == where the projectile is aimed at. Usually it's the cursor's position
 *      for player skills, or the Hero's current location + rand for AI skills.
 */
class Aim {
  public:
    Aim() : origin(0.0f), destination(0.0f) { }
    ugdk::Vector2D origin;
    ugdk::Vector2D destination;
};

class Nothing {};

} // castarguments
} // skills

#endif