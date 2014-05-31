#ifndef HORUSEYE_COMPONENT_GRAPHIC_H_
#define HORUSEYE_COMPONENT_GRAPHIC_H_

#include "game/components/base.h"

#include "game/sprites.h"
#include "game/components.h"
#include "game/components/orders.h"
#include "game/scenes/gamelayer.h"

#include <ugdk/structure/types.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/primitivecontroller.h>

#include <string>
#include <memory>

namespace component {

class Graphic : public Base {
  public:
    static const char* DEFAULT_NAME() { return "graphic"; }
    static int DEFAULT_ORDER() { return orders::GRAPHIC; }

    static Graphic* Create(const std::function<void (ugdk::graphic::Primitive&)>& primitive_prepare_function);

    static Graphic* CreateWithAnimationSet(const std::string& spritesheet_name, const std::string& animation_set);
    static Graphic* CreateWithSingleFrame(const std::string& spritesheet_name, const std::string& frame_name);

    ~Graphic();

    const ugdk::graphic::Primitive& primitive() const { return primitive_; }
    ugdk::graphic::Primitive& primitive() { return primitive_; }

    void set_layer(scene::GameLayer layer) { layer_ = layer; }
    scene::GameLayer layer() const { return layer_; }
    
    /**@arg position The graphic's position, in the game's coordinates.*/
    void SetPosition(const ugdk::math::Vector2D& position);

    const ugdk::math::Vector2D& render_offset() const { return render_offset_; }
    const ugdk::math::Vector2D& final_position() const { return final_position_; }
    void set_render_offset(const ugdk::math::Vector2D& render_offset);

          ugdk::graphic::VisualEffect& visual_effect()       { return visual_effect_; }
    const ugdk::graphic::VisualEffect& visual_effect() const { return visual_effect_; }

    double alpha() const;
    void ChangeAlpha(double alpha);

    bool visible() const;
    void set_visible(bool visible);

    void Update(double dt);

    virtual void OnAdd(sprite::WorldObject*);
    
    Animator* animator() { return animator_; }
    void ChangeToFrame(const std::string& frame_name);
    void ChangeToFrame(std::size_t frame_number);

  private:
    Graphic(Animator* animator);
    void UpdateFinalPosition();

    ugdk::graphic::Primitive primitive_;
    Animator* animator_;

    scene::GameLayer layer_;

    ugdk::math::Vector2D world_position_;
    ugdk::math::Vector2D render_offset_;
    ugdk::math::Vector2D final_position_;
    ugdk::graphic::VisualEffect visual_effect_;

};  // class BaseGraphic

}  // namespace component

#endif  // HORUSEYE_COMPONENT_BASEGRAPHIC_H_
