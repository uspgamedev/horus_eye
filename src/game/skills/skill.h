#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_
#include <ugdk/graphic/image.h>

namespace sprite {
class Creature;
class Skill {
    // This serves the purpose of a TypeClass actually.
    //     (only it also contains as much info as possible)
    // Any Skill type should implement the following two pure virtual functions:
    //     virtual void Use(<args>) = 0;
    //     virtual bool isValidUse(<args>) = 0;
    // The first one is the actual "DoSkill()" function of the Skill
    // The second one is used in Use(..), it returns false if the skill can't be used
    //     with the specific <args> configuration, and Use(..) does nothing in this case.
  public:
    Skill(Creature* owner) : owner_(owner), icon_(NULL) {}
    ugdk::Image* icon() const { return icon_; }
    void set_icon(ugdk::Image* icon) { icon_ = icon; }
    

  protected:
	Creature* owner_;
    ugdk::Image* icon_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_WEAPON_H_ */
