#include "fireball.h"
#include "../scenes/world.h"

using namespace ugdk;
using namespace utils;

namespace sprite {

Fireball::Fireball(Vector2D &dir, Explosion *explosion) : MagicMissile(dir) 
{
    explosion_ = explosion;
}

Fireball::~Fireball() {
}


void Fireball::Explode() {
    if (explosion_ != NULL) {
        WORLD()->AddWorldObject(explosion_, this->world_position());
        explosion_ = NULL;
    }
    MagicMissile::Explode();
}

void Fireball::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

void Fireball::Update(float delta_t) {
	MagicMissile::Update(delta_t);
   // this->Move(delta_t);
}

}
