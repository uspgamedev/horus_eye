//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/scenes/imagescene.cc
// Implementacao da classe ImageScene.
//

#include "imagescene.h"
#include "../../framework/engine.h"
#include "../../framework/layer.h"
#include "../../framework/sprite.h"
#include "../../framework/inputmanager.h"
#include "../utils/levelmanager.h"
#include "../../framework/keys.h"

namespace scene {

#define BG  0
#define IMG 1

using namespace framework;

ImageScene::ImageScene(framework::Image *background, framework::Image *image,
                float time, SceneType type) : time_(time) {


    type_ = type;
    if (background) {
        scene_layers_[BG] = new Layer;
        AddLayer(scene_layers_[BG]); // [0] layer do fundo
        Sprite *bg_sprite = new Sprite;
        bg_sprite->Initialize(background);
        scene_layers_[BG]->AddSprite(bg_sprite);
    }
    else scene_layers_[BG] = NULL;

    if (image) {
        scene_layers_[IMG] = new Layer;
        AddLayer(scene_layers_[IMG]); // [1] layer da imagem
        Sprite *img_sprite = new Sprite;
        img_sprite->Initialize(image);
        Vector2D pos = VIDEO_MANAGER()->video_size();
        pos.x = pos.x/2.0 - img_sprite->image()->width()/2.0;
        img_sprite->set_position(pos);
        scene_layers_[IMG]->AddSprite(img_sprite);
        float delta_h = image->height() + VIDEO_MANAGER()->video_size().y;
        if (delta_h > 0)
            movement_ = Vector2D(0, delta_h/time);
    }
    else scene_layers_[IMG] = NULL;;



}

ImageScene::~ImageScene() {}

void ImageScene::End() {
    Scene::End();
    set_visible(false);
}

void ImageScene::Update(float delta_t) {
    Scene::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    if (input->KeyPressed(K_RETURN) || input->KeyPressed(K_ESCAPE) ||
        input->KeyPressed(K_KP_ENTER) || input->MouseUp(M_BUTTON_LEFT))
        utils::LevelManager::reference()->StartGame(type_);

    if (time_ > 0) {
        if (scene_layers_[IMG]) {
            Layer *img_layer = scene_layers_[IMG];
            Vector2D new_offset = img_layer->offset() + movement_*delta_t;
            img_layer->set_offset(new_offset);
        }
        time_ -= delta_t;
    }
    else utils::LevelManager::reference()->StartGame(type_);

}

}
