#ifndef HORUSEYE_GAME_AI_AI_H_
#define HORUSEYE_GAME_AI_AI_H_

#include "game/ai.h"
#include "game/components/controller.h"

#include <ugdk/script.h>
#include <ugdk/system/compatibility.h>

#include <string>

namespace ai {

class AI : public component::Controller {
  typedef component::Controller super;
  public:

    // States in which an AI can be, generally.
    enum State { CREATED, RUNNING, FINISHED };

    AI(ugdk::script::LangWrapper* script_wrapper, const std::string& name);
    ~AI();

    void Start();
    void Finish();
    
    void Update(double dt) override;
    void OnAdd(sprite::WObjRawPtr) override;

    void set_root(AIModule* root);

          sprite::WObjRawPtr owner()       { return owner_; }
    const sprite::WObjRawPtr owner() const { return owner_; }
    const std::string& name() const { return name_; }
    State state() const { return state_; }
    
    void set_standing(bool standing);
    bool standing();

    bool IsUsingSkillSlot(SkillSlot slot) const;

    const ugdk::math::Vector2D& direction_vector() const;

  protected:
    /// The owner.
    sprite::WObjRawPtr owner_;
    
    std::string name_;
    AIModule* root_;
    AIData* data_;
    State state_;
};

}
#endif // AI_H_
