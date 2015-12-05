#ifndef HORUSEYE_COMPONENT_LIGHT_H_
#define HORUSEYE_COMPONENT_LIGHT_H_

#include "game/components/base.h"

#include <ugdk/structure/color.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#include "game/components/orders.h"

namespace component {

class LightEmitter : public Base {
  public:
    LightEmitter(double light_radius, ugdk::structure::Color color = ugdk::structure::Color(1.0, 1.0, 1.0));
    ~LightEmitter();

    static std::string DEFAULT_NAME() { return "light"; }
    std::string component_name() const override { return DEFAULT_NAME(); }
    int order() const override { return orders::GRAPHIC + 2; }

    /**@arg position The graphic's position, in the game's coordinates.*/
    void SetPosition(const ugdk::math::Vector2D& position);

    double radius() const { return radius_; }
    void ChangeRadius(double radius);

    void ChangeColor(const ugdk::structure::Color& color);

    virtual void Update(double dt);

    void Render(ugdk::graphic::Canvas&) const;
    
    virtual void OnAdd(sprite::WObjRawPtr);

  private:
    ugdk::graphic::Light* light_;

    /// How much light this component emits.
    double radius_;

    /// The light's color.
    ugdk::structure::Color color_;

    /// The light's position, in screen's coordinates.
    ugdk::math::Vector2D position_;

};  // class LightEmitter

}  // namespace component

#endif  // HORUSEYE_COMPONENT_LIGHT_H_
