//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/inputmanager.h
// Definicao da classe InputManager.
//
#ifndef HORUSEYE_FRAMEWORK_INPUTMANAGER_H_
#define HORUSEYE_FRAMEWORK_INPUTMANAGER_H_

#include <SDL/SDL.h>
#include "vector2D.h"

namespace framework {

class InputManager {
  public:
    // Construtores e destrutores
    InputManager();
    ~InputManager() {}

    // Member functions
    void Update(float);
    Vector2D GetMouseState(void);
    void ShowCursor(bool toggle);
    bool KeyPressed(int key);
    bool KeyDown(int key);
    bool KeyUp(int key);
    
  private:
    Uint8 *keystate_now, *keystate_last;
};

}
#endif
