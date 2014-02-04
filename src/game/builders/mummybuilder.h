#ifndef HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_

#include <vector>
#include <string>
#include <ugdk/graphic.h>
#include "game/sprites.h"

namespace builder {

namespace MummyBuilder {

void PrepareBasicMummy(const sprite::ObjectHandle& handle, const std::string& spritesheetname,
                       double life, double radius, double speed, bool standing,
                       const std::string& animation_descriptor = "creature");

sprite::WObjPtr WalkingMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr StandingMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr WalkingRangedMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr StandingRangedMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr WalkingBigMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr StandingBigMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr WalkingPaperMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr StandingPaperMummy(const std::vector<std::string>& arguments);
sprite::WObjPtr WalkingPharaoh(const std::vector<std::string>& arguments);
sprite::WObjPtr StandingPharaoh(const std::vector<std::string>& arguments);

} // namespace MummyBuilder

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_ */
