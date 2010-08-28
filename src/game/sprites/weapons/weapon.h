#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_
namespace sprite {
class Creature;
class Weapon {
  public:
    Weapon(Creature* owner) { owner_ = owner; }
	virtual float range() = 0;
	virtual void Attack() = 0;
	virtual bool Available() = 0;

  protected:
	Creature* owner_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_ */
