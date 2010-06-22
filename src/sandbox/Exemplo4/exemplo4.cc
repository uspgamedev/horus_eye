#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/vector2D.h"
#include "../../game/sprites/hero.h"
#include "../../game/sprites/wall.h"
#include "../../game/scenes/world.h"
#include <cmath>

using namespace std;
using namespace framework;
using namespace scene;
using namespace sprite;

int main(int argc, char *argv[]) {
    Engine *engine = Engine::reference();
    engine->Initialize("Mage", Vector2D(640, 480), false);

    World *world = new World;
    Hero *mage = new Hero;
    Wall *wall = new Wall;

    FILE* ohnoes = fopen("output.txt", "w");
    fprintf(ohnoes, "Exemplo 4 iniciado\n");
    fclose(ohnoes);

    engine->PushScene(world);
    world->AddWorldObject(mage);
    world->AddWorldObject(wall);
    engine->Run();

    engine->Release();

    ohnoes = fopen("output.txt", "a");
    fprintf(ohnoes, "Exemplo 4 encerrado\n");
    fclose(ohnoes);

    return 0;
}

