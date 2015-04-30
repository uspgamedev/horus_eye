#ifndef HORUSEYE_FRONTEND_GAMEVIEW_OBJECTGRAPHIC_H_
#define HORUSEYE_FRONTEND_GAMEVIEW_OBJECTGRAPHIC_H_

#include "game/core/gamelayer.h"

#include <ugdk/structure/types.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/primitivecontroller.h>
#include "game/components/graphic.h"

#include <string>
#include <memory>

namespace frontend {
namespace gameview {

class ObjectGraphic {
  public:
    struct ObjectGraphicRange {
        std::vector<const ObjectGraphic*>::const_iterator begin() const;
        std::vector<const ObjectGraphic*>::const_iterator end() const;
    };
    struct ObjectGraphicOrderedRange {
        std::vector<const ObjectGraphic*>::const_iterator begin() const;
        std::vector<const ObjectGraphic*>::const_iterator end() const;
    };

    static ObjectGraphic* Create(component::Graphic::CreateTypes, const std::string& arg);

    static ObjectGraphic* CreateWithSpritesheet(const std::string& spritesheet_name);
    static ObjectGraphic* CreateWithSingleFrame(const std::string& spritesheet_name, const std::string& frame_name);

    static ObjectGraphicRange CurrentInstances() { return ObjectGraphicRange(); }
    static ObjectGraphicOrderedRange CurrentOrderedInstances() { return ObjectGraphicOrderedRange(); }

    ~ObjectGraphic();

    const ugdk::graphic::Primitive& primitive() const { return primitive_; }

    void set_layer(core::GameLayer layer) { layer_ = layer; }
    core::GameLayer layer() const { return layer_; }
    
    /**@arg position The graphic's position, in the game's coordinates.*/
    void SetPosition(const ugdk::math::Vector2D& position);

    const ugdk::math::Vector2D& world_position() const { return world_position_; }
    const ugdk::math::Vector2D& render_offset() const { return render_offset_; }
    const ugdk::math::Vector2D& final_position() const { return final_position_; }
    void set_render_offset(const ugdk::math::Vector2D& render_offset);

          ugdk::graphic::VisualEffect& visual_effect()       { return visual_effect_; }
    const ugdk::graphic::VisualEffect& visual_effect() const { return visual_effect_; }

    double alpha() const;
    void ChangeAlpha(double alpha);

    bool visible() const;
    void set_visible(bool visible);

    void SetOrderDependent(bool active);

    void ChangeToFrame(const std::string& frame_name);
    void ChangeToFrame(std::size_t frame_number);
    void ChangeToAnimationFrame(const ugdk::graphic::SpriteAnimationFrame& frame);

  private:
    ObjectGraphic();
    void UpdateFinalPosition();

    ugdk::graphic::Primitive primitive_;

    core::GameLayer layer_;

    bool order_dependent_;
    ugdk::math::Vector2D world_position_;
    ugdk::math::Vector2D render_offset_;
    ugdk::math::Vector2D final_position_;
    ugdk::graphic::VisualEffect visual_effect_;

};  // class ObjectGraphic

} // namespace gameview
} // namespace frontend

#endif  // HORUSEYE_FRONTEND_GAMEVIEW_OBJECTGRAPHIC_H_
