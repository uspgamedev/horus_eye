//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/editor/editorobject.h
// Definicao da classe EditorObject.
//

#ifndef HORUSEYE_GAME_UTILS_EDITOR_EDITOROBJECT_H_
#define HORUSEYE_GAME_UTILS_EDITOR_EDITOROBJECT_H_

#include "../../sprites/worldobject.h"
#include "worldeditor.h"

namespace utils {

class EditorObject: public sprite::WorldObject {
  public:
    EditorObject(WorldEditor::ObjectToken);
    virtual ~EditorObject() { }

    void MarkSelected(bool);

    void Update(float dt);

  private:
    WorldEditor::ObjectToken token_;
    bool is_selected_;
};

}

#endif /* HORUSEYE_GAME_UTILS_EDITOR_EDITOROBJECT_H_ */
