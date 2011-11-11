#ifndef HORUSEYE_GAME_SPRITE_PHARAOH_H_
#define HORUSEYE_GAME_SPRITE_PHARAOH_H_

#include "game/sprites/creatures/mummy.h"

namespace ugdk {
	class Image;
	class Vector2D;
}
namespace sprite {

class CombatArt;

class Pharaoh : public Mummy {

  public:
    Pharaoh(ugdk::Image* image, int life, int mana);
    ~Pharaoh();

	void set_ranged_weapon(CombatArt *weapon) { ranged_weapon_ = weapon; }
	void set_summon_weapon(CombatArt *weapon) { summon_weapon_ = weapon; }

	void StartSummonMummy(Creature* target);

  protected:
	CombatArt *ranged_weapon_, *summon_weapon_;

	void Update(float delta_t);
	bool CanAttackWithMeele(Vector2D diff);
	bool CanAttackWithRangedWeapon(Vector2D diff);
    void Think(float dt);

};

}
#endif // PHARAOH_H_
