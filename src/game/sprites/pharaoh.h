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

namespace sprite {

class Mummy;
class Weapon;

class Pharaoh : public Mummy {

  public:
    Pharaoh(int life);
    ~Pharaoh();

	void set_ranged_weapon(Weapon *weapon) { ranged_weapon_ = weapon; }

  protected:
	Weapon *ranged_weapon_;

    void Think(float dt);

};

}
#endif // PHARAOH_H_
