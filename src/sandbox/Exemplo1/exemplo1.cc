
#include "..\..\framework\engine.h"
#include "..\..\framework\scene.h"
#include "..\..\framework\image.h"
#include "..\..\framework\sprite.h"
#include "..\..\framework\videomanager.h"
#include "..\..\framework\inputmanager.h"
#include "..\..\framework\timehandler.h"
#include "..\..\framework\vector2D.h"



using namespace framework;

class Fase1 : public Scene {
    Image *image;

public:
    Fase1() {
        image = Engine::reference()->video_manager()->LoadImage("ryu1.png");
    }
    void Update(float delta_t) {
        image->DrawTo(Engine::reference()->video_manager()->backbuffer(), Vector2D(100,100), 0, Image::MIRROR_NONE);
        if(Engine::reference()->input_manager()->KeyPressed(SDLK_0))
            Engine::reference()->quit();

    }
};



int main(int argc, char* argv[]) {
    Engine * engine = Engine::reference();
    engine->Initialize();

    Fase1 * scene = new Fase1();
  /* Layer *layer = new Layer();
    Image *image = engine->video_manager()->LoadImage("ryu1.png");


    Sprite *sprite = new Sprite(image);

    scene->AddLayer(layer);
    layer->AddSprite(sprite);*/


    engine->PushScene(scene);

    engine->Run();

    return 0;
}
