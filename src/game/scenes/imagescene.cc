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

ImageScene::ImageScene(framework::Image *background, framework::Image *image) {
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
        pos.x = pos.x/2.0f - img_sprite->size().x/2.0f;
        pos.y = 0;
        img_sprite->set_position(pos);
        scene_layers_[IMG]->AddSprite(img_sprite);
        Vector2D layeroffset(0, -(VIDEO_MANAGER()->video_size()*0.5f).y);
        scene_layers_[IMG]->set_offset(layeroffset);
    }
    else scene_layers_[IMG] = NULL;
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
        Finish();
}

}
