#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"
#include "../../framework/vector2D.h"
#include "../../game/sprites/creature.h"
#include "../../game/sprites/hero.h"
#include "../../game/sprites/wall.h"
#include "../../game/scenes/world.h"
#include <cmath>

using namespace std;
using namespace framework;
using namespace scene;
using namespace sprite;

class TestWall : public sprite::Wall {
  public:
	Animation *wall_animation_;
	TestWall() {
		Initialize(Engine::reference()->video_manager()->LoadImage("mage.png"));
		set_position(Vector2D(350, 350));
		collision_radius_ = 10.0f;

		wall_animation_ = new Animation(0, 0, -1);
		SelectSpriteAnimation();
	}

	void SelectSpriteAnimation() {
		Vector2D frame_size(74, 60);
		this->image()->set_frame_size(frame_size);
		this->SelectAnimation(wall_animation_);
	}
};

int main(int argc, char *argv[]) {
    Engine *engine = Engine::reference();
    engine->Initialize("Mage", Vector2D(640, 480), false);

    World *world = new World;
    Hero *mage = new Hero;
    TestWall *wall = new TestWall;

    FILE* ohnoes = fopen("output.txt", "w");
    fprintf(ohnoes, "\nExemplo 4 iniciado\n");
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

