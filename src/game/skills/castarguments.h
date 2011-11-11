#ifndef HORUSEYE_GAME_SKILLS_CASTARGUMENTS_H_
#define HORUSEYE_GAME_SKILLS_CASTARGUMENTS_H_

#include <ugdk/math/vector2D.h>

namespace skills {
namespace castarguments {

struct Aim {
    ugdk::Vector2D origin;
    ugdk::Vector2D destiny;
};

}
}

#endif