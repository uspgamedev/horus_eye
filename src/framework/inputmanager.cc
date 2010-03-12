//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/inputmanager.cc
// Implementacao da classe InputManager.
//
#include "inputmanager.h"

namespace framework {

void InputManager::Update(float delta_t) {
    SDL_PumpEvents();
}

void GetMouseState(int *x, int *y) {
    SDL_GetMouseState(x, y);
}

void WarpMouse(Uint16 x, Uint16 y) {
    SDL_WarpMouse(x, y);
}

int ShowCursor(int toggle) {
    SDL_ShowCursor(toggle);
}

Uint8 *GetKeyState(int *numkeys) {
    return SDL_GetKeyState(numkeys);
}

}
