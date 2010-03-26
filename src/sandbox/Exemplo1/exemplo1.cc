#include "..\..\framework\engine.h"
#include "..\..\framework\scene.h"
#include "..\..\framework\image.h"
#include "..\..\framework\sprite.h"
#include "..\..\framework\videomanager.h"
#include "..\..\framework\inputmanager.h"
#include "..\..\framework\timehandler.h"
#include "..\..\framework\vector2D.h"
#include <cmath>

using namespace std;
using namespace framework;

class Ryu : public Sprite {
  public:
    Ryu() : Sprite(Engine::reference()->video_manager()->LoadImage("ryu_punch.bmp")) {
        Vector2D frameSize(67, 83);
        this->image()->set_frame_size(frameSize);

        punch_animation = new Animation(9, 0, 1, 2, 3, -1);
        standing_animation =  new Animation(1, 0, -1);

        //this->Punch();
    }

    void Punch() {
        this->SelectAnimation(punch_animation);
    }

    void Stand() {
        this->SelectAnimation(standing_animation);
    }

  private:
    Animation *punch_animation;
    Animation *standing_animation;
};

class RyuScene : public Scene {

public:
    RyuScene() {
        ryu = new Ryu();
        video = Engine::reference()->video_manager();
        input = Engine::reference()->input_manager();
        main_layer = new Layer();

        main_layer->AddSprite(ryu);
        this->AddLayer(main_layer);
    }

    void Update(float delta_t) {
        Scene::Update(delta_t);
        video->backbuffer()->Clear(0);
        if(input->KeyDown(SDLK_0))
            Engine::reference()->quit();
        if(input->KeyDown(SDLK_d))
            ryu->Punch();
        if(input->KeyDown(SDLK_s))
            ryu->Stand();
    }

  private:
    Ryu *ryu;
    VideoManager *video;
    Layer *main_layer;
    InputManager *input;
};

int main(int argc, char* argv[]) {
    Engine * engine = Engine::reference();
    engine->Initialize("Ryu Punching", Vector2D(640, 480), false);

    RyuScene * scene = new RyuScene();

    engine->PushScene(scene);

    engine->Run();
    engine->Release();

    return 0;
}
