#ifndef HORUSEYE_GAME_SPRITES_ITEMEVENT_H_
#define HORUSEYE_GAME_SPRITES_ITEMEVENT_H_

namespace sprite {

class Hero;
class ItemEvent {
  public:
    virtual ~ItemEvent () {}
    virtual bool Use (Hero *) = 0;
  protected:
    ItemEvent () {}
};

}

#endif /* HORUSEYE_GAME_SPRITES_ITEMEVENT_H_ */
