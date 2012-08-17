#ifndef HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_

#include <ugdk/graphic.h>
#include "game/sprites.h"

namespace builder {

namespace MummyBuilder {

sprite::WorldObject *WalkingMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *StandingMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *WalkingRangedMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *StandingRangedMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *WalkingBigMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *StandingBigMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *WalkingPaperMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *StandingPaperMummy(const std::vector<std::string>& arguments);
sprite::WorldObject *WalkingPharaoh(const std::vector<std::string>& arguments);
sprite::WorldObject *StandingPharaoh(const std::vector<std::string>& arguments);

};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_ */
