//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/editor/editorobject.cc
// Implementacao da classe EditorObject.
//

#include "editorobject.h"
#include "../circleobject.h"
#include "../../../framework/engine.h"
#include "../../../framework/image.h"
#include "../../../framework/videomanager.h"
#include "../imagefactory.h"
#include "../constants.h"
#include <float.h>

namespace utils {

EditorObject::EditorObject(WorldEditor::ObjectToken token) {
    this->bound_ = new CircleObject(0.01f);
    token_ = token;

    ImageFactory img_fac;
    Image * img;
    switch(token) {
    case WorldEditor::WALL:
        img = VIDEO_MANAGER()->LoadImage("data/images/stoneblock.png");
        img->set_frame_size(Vector2D(71, 74));
        this->set_hotspot(Vector2D(71.0/2, 74.0 - 20.5));
        break;
    case WorldEditor::DOOR:
        img = VIDEO_MANAGER()->LoadImage("data/images/stairs.png");
        img->set_frame_size(Vector2D(Constants::DOOR_SPRITE_WIDTH, Constants::DOOR_SPRITE_HEIGHT));
        this->set_hotspot(Vector2D(Constants::DOOR_SPRITE_WIDTH/2.0, Constants::DOOR_SPRITE_HEIGHT - Constants::DOOR_HOTSPOT_HEIGHT));
        break;
    case WorldEditor::MUMMY:
        img = img_fac.MummyImage();
        img->set_frame_size(Vector2D(74, 74));
        this->set_hotspot(Vector2D(37, 55));
        break;
    case WorldEditor::HERO:
        img = img_fac.HeroImage();
        img->set_frame_size(Vector2D(74, 74));
        this->set_hotspot(Vector2D(37, 55));
        break;
    case WorldEditor::FLOOR:
        img = VIDEO_MANAGER()->LoadImage("data/images/chao41x41.png");
        img->set_frame_size(framework::Vector2D(71,41));
        this->set_hotspot(Vector2D(71.0/2, 41.0/2));
        break;
    default:
        img = NULL;
    }
    Initialize(img);
}

void EditorObject::MarkSelected(bool is_selected) {
    this->is_selected_ = is_selected;
}

void EditorObject::Update(float delta_t) {
    WorldObject::Update(delta_t);
    if(token_ == WorldEditor::FLOOR)
        set_zindex(-FLT_MAX);
}

}
