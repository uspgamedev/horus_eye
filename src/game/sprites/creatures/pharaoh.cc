#include <ugdk/math/vector2D.h>

#include "pharaoh.h"

#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/constants.h"
#include "game/skills/combatart.h"

using namespace std;
using namespace ugdk;
using namespace scene;
using namespace utils;
using resource::Energy;

namespace sprite {

Pharaoh::Pharaoh(Image* image, int life, int mana) : Mummy(image) {
	life_ = Energy(life);
	mana_ = Energy(mana, Constants::PHARAOH_MANA_REGEN);
    identifier_ = "Pharaoh";

    standing_ = true;
    invulnerability_time_ = 1000;
    super_armor_ = true;
}

Pharaoh::~Pharaoh() {
	delete ranged_weapon_;
	delete summon_weapon_;
}

void Pharaoh::Update(float delta_t) {
	if (status_ == WorldObject::STATUS_DEAD) return;
	Mummy::Update(delta_t);

	AdjustBlink(delta_t);
}

}