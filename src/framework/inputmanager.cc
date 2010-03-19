//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/inputmanager.cc
// Implementacao da classe InputManager.
//
#include "inputmanager.h"
#include "vector2d.h"

namespace framework {

InputManager::InputManager() {
    keystate_last = keystate_now = SDL_GetKeyState(NULL);
}

void InputManager::Update(float delta_t) {
    SDL_PumpEvents();
    keystate_last = keystate_now;
    keystate_now = SDL_GetKeyState(NULL);
}

Vector2D GetMouseState(void) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return new Vector2D((float) x, (float) y);
}

void WarpMouse(Uint16 x, Uint16 y) {
    SDL_WarpMouse(x, y);
}

int ShowCursor(int toggle) {
    SDL_ShowCursor(toggle);
}

bool KeyPressed(int key) {
    return (keystate_now[key] && !keystate_last[key]);
}

bool KeyDown(int key) {
    return keystate_now[key];
}

bool KeyUp(int key) {
    return (!keystate_now[key] && keystate_last[key]);
}

} //namespace framework

