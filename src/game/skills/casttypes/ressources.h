#ifndef HORUSEYE_GAME_SKILLS_CASTTYPES_RESSOURCES_H_
#define HORUSEYE_GAME_SKILLS_CASTTYPES_RESSOURCES_H_

namespace skills {
namespace casttypes {

class Aim {

  public:
      Aim& aim() { return aim_; };

  private:
    Vector2D aim_;

};

} // casttypes
} // skills

#endif