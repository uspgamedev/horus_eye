#ifndef HORUSEYE_GAME_SCENES_LIGHTRENDERING_H_
#define HORUSEYE_GAME_SCENES_LIGHTRENDERING_H_

#include <ugdk/action/scene.h>

#include <ugdk/graphic/rendertexture.h>
#include <ugdk/graphic.h>
#include "game/scenes.h"

#include <memory>


namespace scene {

class LightRendering : public ugdk::action::Scene {
  public:
    LightRendering(World*);
    ~LightRendering();

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

} // namespace scene

#endif // HORUSEYE_GAME_SCENES_LIGHTRENDERING_H_