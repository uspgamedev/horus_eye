#ifndef HORUSEYE_COMPONENT_BASEGRAPHIC_H_
#define HORUSEYE_COMPONENT_BASEGRAPHIC_H_

#include "game/components/base.h"

#include <ugdk/structure/types.h>
#include <ugdk/graphic.h>
#include <ugdk/time.h>
#include <ugdk/math/vector2D.h>

#include "game/sprites.h"
#include "game/components/orders.h"
#include "game/scenes/gamelayer.h"

namespace component {

class BaseGraphic : public Base {
  public:
    static const char* DEFAULT_NAME() { return "graphic"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC; }

    BaseGraphic();
    BaseGraphic(ugdk::graphic::Drawable* drawable);
    virtual ~BaseGraphic();

    void set_layer(scene::GameLayer layer) { layer_ = layer; }
    scene::GameLayer layer() const { return layer_; }
    
    /**@arg position The graphic's position, in the game's coordinates.*/
    void SetPosition(const ugdk::math::Vector2D& position);

    void set_render_offset(const ugdk::math::Vector2D& render_offset);

    double alpha() const;
    void ChangeAlpha(double alpha);

    bool visible() const;
    void set_visible(bool visible);

    virtual void Update(double dt);

    virtual void Render(ugdk::graphic::Canvas&) const;

  protected:
    ugdk::graphic::Node* root_node_;
    ugdk::graphic::Node* node_;
    void ChangeDrawable(ugdk::graphic::Drawable*);

  private:
    void setup();

    scene::GameLayer layer_;

    ugdk::math::Vector2D render_offset_;

};  // class BaseGraphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_BASEGRAPHIC_H_
