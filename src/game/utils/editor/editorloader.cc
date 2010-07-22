//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/editor/editorloader.cc
// Implementacao da classe EditorLoader.
//

#include "editorloader.h"
#include <cstdio>

namespace utils {

using namespace std;
using namespace framework;

#define def_WALL    'W'
#define def_DOOR    'D'
#define def_MUMMY   'M'
#define def_HERO    'H'
#define def_FLOOR   'X'
#define def_EMPTY   'O'

WorldEditor::ObjectToken EditorLoader::ConvertToObjectToken(char token) {
    switch(token) {
    case def_WALL: return WorldEditor::WALL;
    case def_DOOR: return WorldEditor::DOOR;
    case def_MUMMY: return WorldEditor::MUMMY;
    case def_HERO: return WorldEditor::HERO;
    case def_FLOOR: return WorldEditor::FLOOR;
    default: return WorldEditor::EMPTY;
    }
}


/*
 * Le o arquivo de texto e passa as informa��es de que objetos criar, e onde,
 * ao World. O arquivo de entrada deve come�ar com dois inteiros que representam
 * o tamanho do mapa seguida da matriz de simbolos associados aos tipos de
 * objeto a serem criados:
 * Wall     : W
 * Door     : D
 * Mummy    : M 
 * Hero     : H
 * Floor    : X
 * Empty    : O
 */
void EditorLoader::Load(string file_name) {

    WorldEditor::ObjectToken token;
    LoadMatrix(file_name);
    WorldEditor::ObjectToken **matrix = world_->level_matrix();
    int width = world_->level_width(),
        height = world_->level_height();

    world_->InitializeLevel(width, height);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            token = ConvertToObjectToken(matrix[i][j]);
            if (token != WorldEditor::EMPTY) {
                world_->LoadToken(j, i, token);
            }
        }
    }
}

} // namespace utils
