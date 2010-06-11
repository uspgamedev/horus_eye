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
#include "keys.h"

namespace framework {

class InputManager {
  public:
    // Construtores e destrutores
    InputManager();
    ~InputManager();

    // Member functions e
    void Update(float);
    Vector2D GetMouseState(void);
    void ShowCursor(bool toggle);
    bool KeyPressed(Key key);
    bool KeyDown(Key key);
    bool KeyUp(Key key);
        
    bool MousePressed(MouseButton button);
    bool MouseDown(MouseButton button);
    bool MouseUp(MouseButton button);
    
    void SimulateKeyPress(Key key);
    void SimulateKeyRelease(Key key);

  private:
    int kbsize_;
    bool *keystate_now_, *keystate_last_;
    bool mousestate_now_[5], mousestate_last_[5];
    Vector2D mouseposition_;
    
    void UpdateDevices();
};

}
#endif
