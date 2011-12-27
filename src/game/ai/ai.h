#ifndef HORUSEYE_GAME_AI_H_
#define HORUSEYE_GAME_AI_H_

#include "aimodule.h"

namespace sprite {
class Creature;
}

namespace ai {

class AI {
  public:

	// States in which an AI can be, generally.
	enum State { CREATED, RUNNING, FINISHED };

	AI(sprite::Creature *owner);
	~AI();

	void Start();
	void Update(float dt);
	void Finish();

	void SetRootModule(AIModule* root);

	State get_state() { return state_; }

  protected:
	sprite::Creature *owner_;
	AIModule *root_;
	State state_;
};

}
#endif // AI_H_