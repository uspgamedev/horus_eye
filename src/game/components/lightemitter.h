#ifndef HORUSEYE_COMPONENT_LIGHT_H_
#define HORUSEYE_COMPONENT_LIGHT_H_

#include "game/components/base.h"

#include <ugdk/structure/types.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#include "game/components/orders.h"

namespace component {

class LightEmitter : public Base {
  public:
    static const char* DEFAULT_NAME() { return "light"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC + 2; }

    LightEmitter(double light_radius, ugdk::Color color = ugdk::Color(1.0, 1.0, 1.0));
    ~LightEmitter();

    /**@arg position The graphic's position, in the game's coordinates.*/
    void SetPosition(const ugdk::math::Vector2D& position);

    double radius() const { return radius_; }
    void ChangeRadius(double radius);

    void ChangeColor(const ugdk::Color& color);

    virtual void Update(double dt);

    void Render(ugdk::graphic::Canvas&) const;
    
    virtual void OnAdd(sprite::WObjRawPtr);

  private:
    ugdk::graphic::Light* light_;

    /// How much light this component emits.
    double radius_;

    /// The light's color.
    ugdk::Color color_;

    /// The light's position, in screen's coordinates.
    ugdk::math::Vector2D position_;

};  // class LightEmitter

}  // namespace component

#endif  // HORUSEYE_COMPONENT_LIGHT_H_
