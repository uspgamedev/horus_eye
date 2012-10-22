#ifndef HORUSEYE_GAME_AI_AI_H_
#define HORUSEYE_GAME_AI_AI_H_

#include <string>
#include <ugdk/script.h>
#include "game/ai.h"
#include "game/components/controller.h"


namespace ai {

class AI : public component::Controller {
  typedef component::Controller super;
  public:

	// States in which an AI can be, generally.
	enum State { CREATED, RUNNING, FINISHED };

	AI(sprite::WorldObject *owner, ugdk::script::LangWrapper* script_wrapper, const std::string& name);
	~AI();

	void Start();
	void Update(double dt);
	void Finish();

	void set_root(AIModule* root);

	const std::string& name() const { return name_; }
	State state() const { return state_; }

    bool IsUsingSkillSlot(SkillSlot slot) const;

    const ugdk::Vector2D& direction_vector() const;

  protected:
    std::string name_;
	AIModule* root_;
    AIData* data_;
	State state_;
};

}
#endif // AI_H_
