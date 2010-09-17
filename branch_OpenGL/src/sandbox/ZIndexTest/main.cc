#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"
#include "../../framework/timeaccumulator.h"
#include "../../framework/vector2D.h"
#include <cmath>

using namespace std;
using namespace framework;










class Mage : public Sprite {
  protected:
    enum Direction { DIRECTION_NW, DIRECTION_NE, DIRECTION_SW, DIRECTION_SE } direction_;

  public:
    Mage(const Vector2D& position) {
        Initialize(Engine::reference()->video_manager()->LoadImage("mage.png"));
        set_position(position);

        standing_down_right =  new Animation(0, 0, -1);
        standing_down_left = new Animation(0, 5, -1);
        standing_up_right = new Animation(0, 10, -1);
        standing_up_left = new Animation(0, 15, -1);

        walking_down_right =  new Animation(10, 0, 1, 2, 3, 4, -1);
        walking_down_left = new Animation(10, 5, 6, 7, 8, 9, -1);
        walking_up_right = new Animation(10, 10, 11, 12, 13, 14, -1);
        walking_up_left = new Animation(10, 15, 16, 17, 18, 19, -1);

        spell_animation =  new Animation(10, 0, 1, 2, 3, 4, -1);

        image()->set_frame_size( Vector2D(74, 60) );
        direction_ = DIRECTION_NW;
        Stand();
    }

    virtual ~Mage() {
        delete standing_down_right;
        delete standing_down_left;
        delete standing_up_right;
        delete standing_up_left;

        delete walking_down_right;
        delete walking_down_left;
        delete walking_up_right;
        delete walking_up_left;

        delete spell_animation;
    }

    virtual void Stand() {
        switch(direction_) {
            case DIRECTION_NW:
                this->SelectAnimation(standing_up_left);
                break;

            case DIRECTION_NE:
                this->SelectAnimation(standing_up_right);
                break;

            case DIRECTION_SW:
                this->SelectAnimation(standing_down_left);
                break;

            case DIRECTION_SE:
                this->SelectAnimation(standing_down_right);
                break;
        }
    }

    virtual void Walk(float dt) {
        Move(dt);
        switch(direction_) {
            case DIRECTION_NW:
                this->SelectAnimation(walking_up_left);
                break;

            case DIRECTION_NE:
                this->SelectAnimation(walking_up_right);
                break;

            case DIRECTION_SW:
                this->SelectAnimation(walking_down_left);
                break;

            case DIRECTION_SE:
                this->SelectAnimation(walking_down_right);
                break;
        }
    }

    virtual void CastSpell() {
        this->SelectAnimation(spell_animation);
    }

    virtual void Update(float dt) {
        Sprite::Update(dt);
        ReadInputDevice(dt);
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

    virtual void ReadInputDevice(float dt) = 0;

    virtual void Move(float delta_t) {
        VideoManager *video = Engine::reference()->video_manager();
        float speed = 50 * delta_t;
        float z = this->zindex();
        Vector2D position(this->position().x, this->position().y);

        switch(direction_) {
            case DIRECTION_NW:
                position.x -= speed;
                position.y -= speed;
                break;

            case DIRECTION_NE:
                position.x += speed;
                position.y -= speed;
                break;

            case DIRECTION_SW:
                position.x -= speed;
                position.y += speed;
                break;

            case DIRECTION_SE:
                position.x += speed;
                position.y += speed;
                break;
        }

        // z e' um valor entre 0.0 e 1.0
        z = position.y / video->video_size().y;

        set_position(position);
        set_zindex(z);
    }
};












class HumanMage : public Mage {
  public:
    HumanMage(const Vector2D position) : Mage(position) {
    }

    virtual void ReadInputDevice(float dt) {
        InputManager* input = Engine::reference()->input_manager();

        if(input->KeyDown(K_SPACE))
            CastSpell();

        if(input->KeyDown(K_s)) {
            direction_ = DIRECTION_SE;
            Walk(dt);
        }

        if(input->KeyDown(K_a)) {
            direction_ = DIRECTION_SW;
            Walk(dt);
        }

        if(input->KeyDown(K_w)) {
            direction_ = DIRECTION_NE;
            Walk(dt);
        }

        if(input->KeyDown(K_q)) {
            direction_ = DIRECTION_NW;
            Walk(dt);
        }
    }
};

class AIMage : public Mage {
  public:
    AIMage(const Vector2D position) : Mage(position), accum(1000) {
    }

    virtual void ReadInputDevice(float dt) {
   
        if(accum.Expired()) {
            direction_ = (Direction)((direction_+1)%4);
            accum.Restart();
        }
      

        Walk(dt);
    }

  private:
    TimeAccumulator accum;
};






class BlackLayer : public Layer {
  public:
    void Render() {
        VideoManager *video = Engine::reference()->video_manager();
        video->backbuffer()->Clear(0);
        Layer::Render();
    }
};






class MageScene : public Scene {
  public:
    MageScene() {
        mage_ = new HumanMage( Vector2D(280,210) );
        mage2_ = new AIMage( Vector2D(280,210) );

        main_layer_ = new BlackLayer();
        main_layer_->AddSprite(mage_);
        main_layer_->AddSprite(mage2_);
        this->AddLayer(main_layer_);
    }

    ~MageScene() {
        delete main_layer_;
        delete mage2_;
        delete mage_;
    }

    void Update(float delta_t) {
        Scene::Update(delta_t);
        HandleKeys();
    }

  private:
    Mage *mage_, *mage2_;
    Layer *main_layer_;

    void HandleKeys() {
        InputManager *input = Engine::reference()->input_manager();
        //if(input->KeyPressed(K_ESCAPE)) // nao funciona pq?!
        if(input->KeyDown(K_ESCAPE))
            Engine::reference()->quit();
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
