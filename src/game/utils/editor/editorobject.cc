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
        img = img_fac.WallImage();
        this->set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y));
        break;
    case WorldEditor::DOOR:
        img = img_fac.DoorImage();
        this->set_hotspot(Vector2D(Constants::DOOR_HOTSPOT_WIDTH, Constants::DOOR_HOTSPOT_HEIGHT));
        break;
    case WorldEditor::MUMMY:
        img = img_fac.MummyImage();
        this->set_hotspot(Vector2D(img->frame_size().x / 2.0,
                                    img->frame_size().y*6.0 / 7.0));
        break;
    case WorldEditor::HERO:
        img = img_fac.HeroImage();
        this->set_hotspot(Vector2D(Constants::HERO_HOTSPOT_X, Constants::HERO_HOTSPOT_Y));
        break;
    case WorldEditor::FLOOR:
        img = img_fac.FloorImage();
        this->set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));
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
