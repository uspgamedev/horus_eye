

#ifndef ITEMEVENT_H_
#define ITEMEVENT_H_

#include "hero.h"

namespace sprite {

class ItemEvent {
  public:
    virtual ~ItemEvent () {}
    virtual bool Use (Hero *) = 0;
  protected:
    ItemEvent () {}
};

}

#endif /* ITEMEVENT_H_ */
