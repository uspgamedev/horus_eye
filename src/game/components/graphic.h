#ifndef HORUSEYE_COMPONENT_GRAPHIC_H_
#define HORUSEYE_COMPONENT_GRAPHIC_H_

#include "game/components/base.h"

#include "game/sprites.h"
#include "game/components.h"
#include "game/components/orders.h"
#include "game/core/gamelayer.h"

#include <ugdk/structure/types.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#include <string>
#include <memory>

namespace frontend {
    namespace gameview {
        class ObjectGraphic;
    }
}

namespace component {

class Graphic : public Base {
  public:
    enum class CreateTypes {
        SPECIALWALL
    };
    static Graphic* Create(CreateTypes, const std::string& arg);

    static Graphic* CreateWithSpritesheet(const std::string& spritesheet_name);
    static Graphic* CreateWithSingleFrame(const std::string& spritesheet_name, const std::string& frame_name);

    ~Graphic();

    static std::string DEFAULT_NAME() { return "graphic"; }
    std::string component_name() const override { return DEFAULT_NAME(); }
    int order() const override { return orders::GRAPHIC; }

    void set_layer(core::GameLayer layer);
    
    /**@arg position The graphic's position, in the game's coordinates.*/
    void SetPosition(const ugdk::math::Vector2D& position);

    ugdk::math::Vector2D render_offset() const;
    void set_render_offset(const ugdk::math::Vector2D& render_offset);

    double alpha() const;
    void ChangeAlpha(double alpha);

    bool visible() const;
    void set_visible(bool visible);

    void Update(double dt) override;
    virtual void OnAdd(sprite::WObjRawPtr) override;
    virtual void OnObjectRemoved() override;

    /// Sets if the order this object is rendered is important.
    /** Ordered rendering is used primarily for transparency.
      * This should be set to true if the object has pixels with
      * alpha between 0 and 1. */
    void SetOrderDependent(bool active);
    
    void ChangeToFrame(const std::string& frame_name);
    void ChangeToFrame(std::size_t frame_number);
    void ChangeToAnimationFrame(const ugdk::graphic::SpriteAnimationFrame& frame);

  private:
    Graphic(frontend::gameview::ObjectGraphic*);
    std::unique_ptr<frontend::gameview::ObjectGraphic> frontend_graphic_;

};  // class BaseGraphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_BASEGRAPHIC_H_
