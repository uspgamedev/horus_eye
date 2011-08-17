#ifndef HORUSEYE_GAME_SPRITE_PHARAOH_H_
#define HORUSEYE_GAME_SPRITE_PHARAOH_H_
#include "mummy.h"
namespace ugdk {
	class Image;
	class Vector2D;
}
namespace sprite {

class Weapon;

class Pharaoh : public Mummy {

  public:
    Pharaoh(ugdk::Image* image, int life, int mana);
    ~Pharaoh();

	void TakeDamage(int life_points = 1);
	void set_ranged_weapon(Weapon *weapon) { ranged_weapon_ = weapon; }
	void set_summon_weapon(Weapon *weapon) { summon_weapon_ = weapon; }

	void StartSummonMummy(Creature* target);

  protected:
	Weapon *ranged_weapon_, *summon_weapon_;
	double mana_regen_time_;

	void Update(float delta_t);
	bool CanAttackWithMeele(Vector2D diff);
	bool CanAttackWithRangedWeapon(Vector2D diff);
    void Think(float dt);

};

}
#endif // PHARAOH_H_
