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

    const ugdk::graphic::GLTexture* light_texture() const;
    ugdk::math::Vector2D CalculateUV(const ugdk::math::Vector2D&) const;

  private:
    void ShadowCasting();
    void ApplyShadowCasting(ugdk::graphic::Canvas& canvas);

    ugdk::graphic::RenderTexture shadow_buffer_;
    ugdk::graphic::RenderTexture light_buffer_;
    bool shadowcasting_actiavated_;
    bool lightsystem_activated_;
    core::World* world_;
};

} // namespace gameview
} // namespace frontend


#endif // HORUSEYE_FRONTEND_GAMEVIEW_LIGHTRENDERING_H_
