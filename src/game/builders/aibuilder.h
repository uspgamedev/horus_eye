#ifndef HORUS_EYE_GAME_BUILDERS_AI_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_AI_BUILDER_H_

namespace ai {
class AI;
}
namespace sprite {
class Mummy;
class Pharaoh;
}

namespace builder {

class AIBuilder
{
public:
	AIBuilder() {}

	ai::AI* BasicMummyAI(sprite::Mummy* owner);
	ai::AI* BasicPharaohAI(sprite::Pharaoh* owner);
};

}
#endif /* HORUS_EYE_GAME_BUILDERS_AI_BUILDER_H_ */