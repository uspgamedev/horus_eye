
#include "..\..\framework\engine.h"
#include "..\..\framework\scene.h"
#include "..\..\framework\image.h"
#include "..\..\framework\sprite.h"
#include "..\..\framework\videomanager.h"
#include "..\..\framework\inputmanager.h"
#include "..\..\framework\timehandler.h"
#include "..\..\framework\vector2D.h"

using namespace framework;

int main(int argc, char* argv[]) {
    Engine * engine = Engine::reference();
    engine->Initialize();

    Scene * scene = new Scene();
  /* Layer *layer = new Layer();
    Image *image = engine->video_manager()->LoadImage("ryu1.png");


    Sprite *sprite = new Sprite(image);

    scene->AddLayer(layer);
    layer->AddSprite(sprite);*/


    engine->PushScene(scene);

    engine->Run();

    return 0;
}
