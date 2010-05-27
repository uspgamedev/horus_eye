#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"
#include "../../framework/vector2D.h"
#include <cmath>

using namespace std;
using namespace framework;

class Mage : public Sprite {
  public:
    int direction_;

    Mage() : Sprite(Engine::reference()->video_manager()->LoadImage("mage.png"), Vector2D(280, 210)) {
        standing_down_right =  new Animation(0, 0, -1);
        standing_down_left = new Animation(0, 5, -1);
        standing_up_right = new Animation(0, 10, -1);
        standing_up_left = new Animation(0, 15, -1);

        walking_down_right =  new Animation(10, 0, 1, 2, 3, 4, -1);
        walking_down_left = new Animation(10, 5, 6, 7, 8, 9, -1);
        walking_up_right = new Animation(10, 10, 11, 12, 13, 14, -1);
        walking_up_left = new Animation(10, 15, 16, 17, 18, 19, -1);
        spell_animation =  new Animation(10, 0, 1, 2, 3, 4, -1);

        direction_ = 0;
        Stand();
    }

    void Stand() {
        is_walking_ = false;
        Vector2D frameSize(74, 60);
        this->image()->set_frame_size(frameSize);
        if (direction_ == 0) {
            this->SelectAnimation(standing_down_right);
        } else if (direction_ == 1) {
            this->SelectAnimation(standing_down_left);
        } else if (direction_ == 2) {
            this->SelectAnimation(standing_up_right);
        } else {
            this->SelectAnimation(standing_up_left);
        }
    }

    void Move() {
        float speed = 0.1;

        Vector2D position(this->position().x, this->position().y);
        if (is_walking_) {
            if (direction_ == 0) {
                position.x += speed;
                position.y += speed;
            } else if (direction_ == 1) {
                position.x -= speed;
                position.y += speed;
            } else if (direction_ == 2) {
                position.x += speed;
                position.y -= speed;
            } else {
                position.x -= speed;
                position.y -= speed;
            }
        }
        set_position(position);
    }

    void Walk() {
        is_walking_ = true;
        Vector2D frameSize(74, 60);
        this->image()->set_frame_size(frameSize);
        if (direction_ == 0) {
            this->SelectAnimation(walking_down_right);
        } else if (direction_ == 1) {
            this->SelectAnimation(walking_down_left);
        } else if (direction_ == 2) {
            this->SelectAnimation(walking_up_right);
        } else {
            this->SelectAnimation(walking_up_left);
        }
    }

    void Spell() {
        Vector2D frameSize(74, 60);
        this->image()->set_frame_size(frameSize);
        this->SelectAnimation(spell_animation);
    }

  private:
    Animation *standing_down_right;
    Animation *standing_down_left;
    Animation *standing_up_right;
    Animation *standing_up_left;

    Animation *walking_down_right;
    Animation *walking_down_left;
    Animation *walking_up_right;
    Animation *walking_up_left;

    Animation *spell_animation;

    bool is_walking_;
};

class MageScene : public Scene {

public:
    MageScene() {
        mage_ = new Mage();
        video_ = Engine::reference()->video_manager();
        input_ = Engine::reference()->input_manager();
        main_layer_ = new Layer();

        main_layer_->AddSprite(mage_);
        this->AddLayer(main_layer_);
    }


    void Update(float delta_t) {
        Scene::Update(delta_t);
        video_->backbuffer()->Clear(0);
        Get_keys();
        mage_->Move();
    }

  private:
    Mage *mage_;
    VideoManager *video_;
    Layer *main_layer_;
    InputManager *input_;

    void Get_keys() {
        if(input_->KeyDown(SDLK_ESCAPE))
            Engine::reference()->quit();
        if(input_->KeyDown(SDLK_SPACE))
            mage_->Spell();

        if(input_->KeyDown(K_f)) {
            mage_->direction_ = 0;
            mage_->Stand();
        }
        if(input_->KeyDown(K_d)) {
            mage_->direction_ = 1;
            mage_->Stand();
        }
        if(input_->KeyDown(K_r)) {
            mage_->direction_ = 2;
            mage_->Stand();
        }
        if(input_->KeyDown(K_e)) {
            mage_->direction_ = 3;
            mage_->Stand();
        }

        if(input_->KeyDown(K_s)) {
            mage_->direction_ = 0;
            mage_->Walk();
        }
        if(input_->KeyDown(K_a)) {
            mage_->direction_ = 1;
            mage_->Walk();
        }
        if(input_->KeyDown(K_w)) {
            mage_->direction_ = 2;
            mage_->Walk();
        }
        if(input_->KeyDown(K_q)) {
            mage_->direction_ = 3;
            mage_->Walk();
        }
    }
};

int main(int argc, char* argv[]) {
    Engine * engine = Engine::reference();
    engine->Initialize("Mage", Vector2D(640, 480), false);

    MageScene * scene = new MageScene();

    engine->PushScene(scene);

    engine->Run();
    engine->Release();

    return 0;
}
