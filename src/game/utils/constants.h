//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/constants.h
// Definicao da classe Constants.
//

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace utils {

class Constants {
  public:

    const static int   PROJECTILE_SPRITE_CENTER_X = 16,
                       PROJECTILE_SPRITE_CENTER_Y = 16,
                       PROJECTILE_HEIGHT = 12,
                       DOOR_SPRITE_WIDTH = 169,
                       DOOR_SPRITE_HEIGHT = 147,
                       DOOR_HOTSPOT_HEIGHT = 55,
                       DOOR_BOUND_WIDTH = 134/41.0,
                       DOOR_BOUND_HEIGHT = 78/41.0;

  private:
    Constants() {}
};

}

#endif /* CONSTANTS_H_ */
