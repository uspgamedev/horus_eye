//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.h
// Definicao da classe Hero.
//

#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_

#include "creature.h"

namespace sprite {

class Creature;

// Grupo 1: Criamos essa classe para podermos fazer a classe World.
class Hero : public Creature {

  public:
    Hero() {}
    virtual ~Hero() {}

};


} // namespace sprite

#endif // HERO_H_
