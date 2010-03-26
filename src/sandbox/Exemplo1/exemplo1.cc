
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

class Fase1 : public Scene {
    Image *image;
    Vector2D pos;

public:
    Fase1() : pos(100,100) {
        image = Engine::reference()->video_manager()->LoadImage("heli.png");
    }
    void Update(float delta_t) {
        VideoManager *video = Engine::reference()->video_manager();
        video->backbuffer()->Clear(0);
        image->DrawTo(Engine::reference()->video_manager()->backbuffer(), pos, 0, Image::MIRROR_NONE);
        pos.set_x(100 - 50*sin(M_PI * (Engine::reference()->time_handler()->TimeElapsed())*0.001));
        pos.set_y(200 + 150*cos(M_PI * (Engine::reference()->time_handler()->TimeElapsed())*0.001));
        if(Engine::reference()->input_manager()->KeyPressed(SDLK_0))
            Engine::reference()->quit();
    }
};



int main(int argc, char* argv[]) {
    Engine * engine = Engine::reference();
    engine->Initialize("exemplo1 :P", Vector2D(640, 480), false);

    Fase1 * scene = new Fase1();
  /* Layer *layer = new Layer();
    Image *image = engine->video_manager()->LoadImage("ryu1.png");


    Sprite *sprite = new Sprite(image);

    scene->AddLayer(layer);
    layer->AddSprite(sprite);*/


    engine->PushScene(scene);

    engine->Run();
    engine->Release();

    return 0;
}
