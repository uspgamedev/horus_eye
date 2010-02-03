//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/frame.cc
// Implementacao da classe Frame.
//
#include "frame.h"

namespace framework {

// Devolve true se houver colisao entre
// 'frame' e este retangulo
bool Frame::Collides(const Frame& frame) const {
    // se houver colisao no eixo-x...
    if(left() < frame.right() && right() > frame.left()) {
        // e se houver colisao no eixo-y...
        if(top() < frame.bottom() && bottom() > frame.top()) {
            return true;
        }
    }

    return false;
}

}  // namespace framework
