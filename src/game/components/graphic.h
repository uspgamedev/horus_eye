#ifndef HORUSEYE_COMPONENT_GRAPHIC_H_
#define HORUSEYE_COMPONENT_GRAPHIC_H_

#include "game/components/base.h"

#include <ugdk/graphic.h>
#include <ugdk/time.h>

#include "game/sprites.h"
#include "game/components/orders.h"
#include "game/scenes/gamelayer.h"

namespace component {

class Graphic : public Base {
  public:
    static const char* DEFAULT_NAME() { return "graphic"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC; }

    Graphic(sprite::WorldObject* owner);
    virtual ~Graphic();

    void set_layer(scene::GameLayer layer) { layer_ = layer; }
    scene::GameLayer layer() const { return layer_; }

          ugdk::graphic::Node* node()       { return node_; }
    const ugdk::graphic::Node* node() const { return node_; }
    
    double light_radius() const { return light_radius_; }
    void ChangeLightRadius(double radius);

    void Update(double dt) { adjustBlink(); }

    void StartBlinking(int duration = -1);
    void StopBlinking();

    void InsertIntoLayers(ugdk::graphic::Node** layers);
    void RemoveFromLayers(ugdk::graphic::Node** layers);

  protected:
    ugdk::graphic::Node* node_;

    scene::GameLayer layer_;

  private:
    void adjustBlink();

    /// The owner.
    sprite::WorldObject* owner_;

    bool is_blinking_;

    /// Controls when to toggle the blink_ flag.
    ugdk::time::TimeAccumulator *blink_time_;

    /// Controls when to stop blinking.
    ugdk::time::TimeAccumulator *blink_duration_;

    /// When true, this component is on the invisible part of the blinking effect.
    bool blink_;

    /// How much light this component emits.
    double light_radius_;

};  // class Graphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_GRAPHIC_H_
