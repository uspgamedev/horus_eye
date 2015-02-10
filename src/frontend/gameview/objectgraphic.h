#ifndef HORUSEYE_FRONTEND_GAMEVIEW_OBJECTGRAPHIC_H_
#define HORUSEYE_FRONTEND_GAMEVIEW_OBJECTGRAPHIC_H_

#include "game/core/gamelayer.h"

#include <ugdk/structure/types.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/primitivecontroller.h>

#include <string>
#include <memory>

namespace frontend {
namespace gameview {

class ObjectGraphic {
  public:
    enum class CreateTypes {
        SPECIALWALL
    };
    static ObjectGraphic* Create(CreateTypes, const std::string& arg);

    static ObjectGraphic* CreateWithSpritesheet(const std::string& spritesheet_name);
    static ObjectGraphic* CreateWithSingleFrame(const std::string& spritesheet_name, const std::string& frame_name);

    static const std::vector<const ObjectGraphic*>& CurrentInstances();

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

    void ChangeToFrame(const std::string& frame_name);
    void ChangeToFrame(std::size_t frame_number);
    void ChangeToAnimationFrame(const ugdk::graphic::SpriteAnimationFrame& frame);

  private:
    ObjectGraphic();
    void UpdateFinalPosition();

    ugdk::graphic::Primitive primitive_;

    core::GameLayer layer_;

    ugdk::math::Vector2D world_position_;
    ugdk::math::Vector2D render_offset_;
    ugdk::math::Vector2D final_position_;
    ugdk::graphic::VisualEffect visual_effect_;

};  // class ObjectGraphic

} // namespace gameview
} // namespace frontend

#endif  // HORUSEYE_FRONTEND_GAMEVIEW_OBJECTGRAPHIC_H_
