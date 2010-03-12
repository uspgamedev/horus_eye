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

namespace framework {

class InputManager {
  public:
    // Construtores e destrutores
    InputManager() {}
    ~InputManager() {}

    // Member functions
    void Update(float);
    void GetMouseState(int *x, int *y);
    void WarpMouse(Uint16 x, Uint16 y);
    int ShowCursor(int toggle);
    Uint8 *GetKeyState(int *numkeys);
};

}
#endif
