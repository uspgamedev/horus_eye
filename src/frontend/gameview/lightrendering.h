#ifndef HORUSEYE_FRONTEND_GAMEVIEW_LIGHTRENDERING_H_
#define HORUSEYE_FRONTEND_GAMEVIEW_LIGHTRENDERING_H_

#include <ugdk/action/scene.h>

#include <ugdk/graphic/rendertexture.h>
#include <ugdk/graphic.h>
#include "game/core.h"

#include <memory>

namespace frontend {
namespace gameview {

class LightRendering {
  public:
    LightRendering(core::World*);
    ~LightRendering();

    void UpdateBuffers();

    void ToggleShadowcasting();
    void ToggleLightsystem();

    const ugdk::math::Vector2D& light_precision() const { return light_precision_; }
    const ugdk::graphic::GLTexture* light_texture() const;
    const ugdk::graphic::GLTexture* shadow_texture() const;
    ugdk::math::Vector2D CalculateUV(const ugdk::math::Vector2D&) const;

    void set_focused_position(const ugdk::math::Vector2D& pos) { focused_position_ = pos; }

  private:
    void ShadowCasting();
    void ApplyShadowCasting(ugdk::graphic::Canvas& canvas);

    ugdk::graphic::RenderTexture shadow_buffer_;
    ugdk::graphic::RenderTexture light_buffer_;
    bool shadowcasting_actiavated_;
    bool lightsystem_activated_;
    core::World* world_;
    ugdk::math::Vector2D focused_position_;
    ugdk::math::Vector2D light_precision_;
};

} // namespace gameview
} // namespace frontend


#endif // HORUSEYE_FRONTEND_GAMEVIEW_LIGHTRENDERING_H_
