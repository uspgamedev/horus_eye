//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/inputmanager.cc
// Implementacao da classe InputManager.
//
#include "inputmanager.h"

namespace framework {

InputManager::InputManager() {
    keystate_last = keystate_now = SDL_GetKeyState(NULL);
}

void InputManager::Update(float delta_t) {
    SDL_PumpEvents();
    keystate_last = keystate_now;
    keystate_now = SDL_GetKeyState(NULL);
}

Vector2D InputManager::GetMouseState(void) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return Vector2D((float) x, (float) y);
}

void InputManager::ShowCursor(bool toggle) {
    SDL_ShowCursor((int) toggle);
}

bool InputManager::KeyPressed(int key) {
    return (keystate_now[key] && !keystate_last[key]);
}

bool InputManager::KeyDown(int key) {
    return keystate_now[key];
}

bool InputManager::KeyUp(int key) {
    return (!keystate_now[key] && keystate_last[key]);
}

} //namespace framework

