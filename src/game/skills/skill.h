#ifndef HORUSEYE_GAME_SKILLS_SKILL_H_
#define HORUSEYE_GAME_SKILLS_SKILL_H_
#include <ugdk/graphic/image.h>

namespace sprite {
class Creature;
} // sprite

namespace skills {

class Skill {
  public:
    Skill(sprite::Creature* owner) : owner_(owner), icon_(NULL) {}
    ugdk::Image* icon() const { return icon_; }
    void set_icon(ugdk::Image* icon) { icon_ = icon; }

    virtual void Use() = 0;
    virtual bool IsValidUse() const = 0;

  protected:
	sprite::Creature* owner_;
    ugdk::Image* icon_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_SKILL_H_ */
