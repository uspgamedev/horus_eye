#ifndef HORUSEYE_COMPONENT_GRAPHIC_H_
#define HORUSEYE_COMPONENT_GRAPHIC_H_

#include "game/components/base.h"

#include <ugdk/graphic.h>
#include <ugdk/time.h>

#include "game/sprites.h"

namespace component {

class Graphic : public Base {
  public:
    static const char* DEFAULT_NAME() { return "graphic"; }

    Graphic(sprite::WorldObject* owner);
    virtual ~Graphic();

          ugdk::graphic::Node* node()       { return node_; }
    const ugdk::graphic::Node* node() const { return node_; }

    void Update(double dt) { AdjustBlink(); }

    void StartBlinking(int duration = -1);
    void StopBlinking();

  protected:
    ugdk::graphic::Node* node_;

  private:
    void AdjustBlink();

    /// The owner.
    sprite::WorldObject* owner_;

    bool is_blinking_;

    /// Controls when to toggle the blink_ flag.
    ugdk::time::TimeAccumulator *blink_time_;

    /// Controls when to stop blinking.
    ugdk::time::TimeAccumulator *blink_duration_;

    /// When true, this component is on the invisible part of the blinking effect.
    bool blink_;

};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
