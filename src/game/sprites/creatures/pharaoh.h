#ifndef HORUSEYE_GAME_SPRITE_PHARAOH_H_
#define HORUSEYE_GAME_SPRITE_PHARAOH_H_

#include "game/sprites/creatures/mummy.h"
#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace ugdk {
class Image;
class Vector2D;
}

namespace skills {
class Skill;
}

namespace sprite {

class Pharaoh : public Mummy {

  public:
    Pharaoh(ugdk::Image* image, int life, int mana);
    ~Pharaoh();

	skills::Skill* get_ranged_weapon() { return ranged_weapon_; }
	void set_ranged_weapon(skills::Skill *weapon) { ranged_weapon_ = weapon; }
	skills::Skill* get_summon_weapon() { return summon_weapon_; }
	void set_summon_weapon(skills::Skill *weapon) { summon_weapon_ = weapon; }

	void StartSummonMummy(sprite::Creature* target);

  protected:
	skills::Skill *ranged_weapon_, *summon_weapon_;

	void Update(float delta_t);

};

}
#endif // PHARAOH_H_