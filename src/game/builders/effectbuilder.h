#ifndef HORUSEYE_GAME_BUILDERS_EFFECTSBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_EFFECTSBUILDER_H_

#include <ugdk/portable/tr1.h>
#include FROM_TR1(memory)

#include "game/sprites.h"

namespace builder {
namespace EffectBuilder {

std::tr1::shared_ptr<sprite::Effect> increase_sight();

}
}

#endif /* HORUSEYE_GAME_BUILDERS_EFFECTSBUILDER_H_ */
