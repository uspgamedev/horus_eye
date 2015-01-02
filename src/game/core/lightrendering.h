#ifndef HORUSEYE_GAME_CORE_LIGHTRENDERING_H_
#define HORUSEYE_GAME_CORE_LIGHTRENDERING_H_

#include <ugdk/action/scene.h>

#include <ugdk/graphic/rendertexture.h>
#include <ugdk/graphic.h>
#include "game/core.h"

#include <memory>

namespace core {

class LightRendering {
  public:
    LightRendering(World*);
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
    World* world_;
};

} // namespace core

#endif // HORUSEYE_GAME_CORE_LIGHTRENDERING_H_
