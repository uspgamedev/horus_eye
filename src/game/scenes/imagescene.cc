#include "imagescene.h"
#include <ugdk/base/engine.h>
#include <ugdk/action/layer.h>
#include <ugdk/action/sprite.h>
#include <ugdk/input/inputmanager.h>
#include "game/utils/levelmanager.h"
#include <ugdk/input/keys.h>

namespace scene {

#define BG  0
#define IMG 1

using namespace ugdk;

ImageScene::ImageScene(ugdk::Image *background, ugdk::Image *image) {
    if (background) {
        scene_layers_[BG] = new Layer;
        Engine::reference()->PushInterface(scene_layers_[BG]); // [0] layer do fundo
        Sprite *bg_sprite = new Sprite;
        bg_sprite->Initialize(background);
        scene_layers_[BG]->AddSprite(bg_sprite);
    }
    else scene_layers_[BG] = NULL;

    if (image) {
        scene_layers_[IMG] = new Layer;
        Engine::reference()->PushInterface(scene_layers_[IMG]); // [1] layer da imagem
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
    if(scene_layers_[BG] != NULL) {
        Engine::reference()->RemoveInterface(scene_layers_[BG]);
        delete scene_layers_[BG];
    }
    if(scene_layers_[IMG] != NULL) {
        Engine::reference()->RemoveInterface(scene_layers_[IMG]);
        delete scene_layers_[IMG];
    }
}

void ImageScene::Update(float delta_t) {
    Scene::Update(delta_t);
    InputManager *input = Engine::reference()->input_manager();
    if (input->KeyPressed(K_RETURN) || input->KeyPressed(K_ESCAPE) ||
        input->KeyPressed(K_KP_ENTER) || input->MouseUp(M_BUTTON_LEFT))
        Finish();
}

}
