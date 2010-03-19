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
    keystate_now = SDL_GetKeyState(&kbsize);
    keystate_last = new Uint8[kbsize];
    Update(1.3);
}

InputManager::~InputManager() {
    delete[] keystate_last;
}

void InputManager::Update(float delta_t) {
    for (int i = 0; i < kbsize; i++) keystate_last[i] = keystate_now[i];
    SDL_PumpEvents();
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

