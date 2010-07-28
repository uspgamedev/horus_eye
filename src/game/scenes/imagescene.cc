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
#include "../../framework/keys.h"

namespace scene {

#define BG  0
#define IMG 1

using namespace framework;

ImageScene::ImageScene(framework::Image *background, framework::Image *image,
                float time) : time_(time) {

    scene_layers_[BG] = new Layer;
    scene_layers_[IMG] = new Layer;

    AddLayer(scene_layers_[BG]); // [0] layer do fundo
    AddLayer(scene_layers_[IMG]); // [1] layer da imagem

    Sprite *bg_sprite = new Sprite;
    bg_sprite->Initialize(background);

    Sprite *img_sprite = new Sprite;
    img_sprite->Initialize(image);

    scene_layers_[BG]->AddSprite(bg_sprite);
    scene_layers_[IMG]->AddSprite(img_sprite);

    float delta_h = image->height() - VIDEO_MANAGER()->video_size().y;
    if (delta_h > 0)
        movement_ = Vector2D(0, delta_h/time);

}

ImageScene::~ImageScene() {}

void ImageScene::End() {
    Scene::End();
    list<Layer*>::iterator i = layers_.begin();
    while (i != layers_.end()) {
        (*i)->set_visible(false);
        ++i;
    }
}

void ImageScene::Update(float delta_t) {
    Scene::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    if (input->KeyPressed(K_RETURN)) Finish();

    if (time_ > 0) {
        Layer *img_layer = scene_layers_[IMG];
        Vector2D new_offset = img_layer->offset() + movement_*delta_t;
        img_layer->set_offset(new_offset);
        time_ -= delta_t;
    }
}

}
