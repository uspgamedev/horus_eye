//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/levelloader.h
// Definicao da classe LevelLoader.
//

#ifndef HORUSEYE_GAME_UTILS_EDITOR_EDITORLOADER_H_
#define HORUSEYE_GAME_UTILS_EDITOR_EDITORLOADER_H_

#include <string>
#include "worldeditor.h"
#include "../levelloader.h"

namespace utils {

class EditorLoader : public LevelLoader {
  public:
    EditorLoader(WorldEditor * world) : LevelLoader(NULL) { world_ = world; }
    virtual ~EditorLoader() {}

    static WorldEditor::ObjectToken ConvertToObjectToken(char token);

    void Load(std::string);

  protected:

    WorldEditor * world_;

};

}

#endif // HORUSEYE_GAME_UTILS_EDITOR_EDITORLOADER_H_
