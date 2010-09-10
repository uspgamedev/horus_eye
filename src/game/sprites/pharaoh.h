//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/pharaoh.h
// Definicao da classe Pharaoh.
//

#ifndef HORUSEYE_GAME_SPRITE_PHARAOH_H_
#define HORUSEYE_GAME_SPRITE_PHARAOH_H_


#define PHARAOH_TIME_TO_THINK 0.05f
namespace framework {
	class Image;
}
namespace sprite {

class Mummy;
class Weapon;

class Pharaoh : public Mummy {

  public:
    Pharaoh(framework::Image* image, int life, int mana);
    ~Pharaoh();

	void TakeDamage(int life_points = 1);
	void set_ranged_weapon(Weapon *weapon) { ranged_weapon_ = weapon; }
	void set_summon_weapon(Weapon *weapon) { summon_weapon_ = weapon; }

	void StartSummonMummy(Creature* target);

  protected:
	Weapon *ranged_weapon_, *summon_weapon_;
	double mana_regen_time_;

	void Update(float delta_t);
    void Think(float dt);

};

}
#endif // PHARAOH_H_
