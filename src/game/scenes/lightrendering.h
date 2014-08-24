#ifndef HORUSEYE_GAME_SCENES_LIGHTRENDERING_H_
#define HORUSEYE_GAME_SCENES_LIGHTRENDERING_H_

#include <ugdk/action/scene.h>

#include <ugdk/graphic/rendertexture.h>
#include <ugdk/internal.h>
#include "game/scenes.h"

#include <memory>


namespace scene {

class LightRendering : public ugdk::action::Scene {
  public:
    LightRendering(World*);
    ~LightRendering();

    void ToggleShadowcasting();
    void ToggleLightsystem();

    const ugdk::internal::GLTexture* light_texture() const;

  private:
    void ShadowCasting();
    void LightMerging();

    ugdk::graphic::RenderTexture shadow_buffer_;
    ugdk::graphic::RenderTexture light_buffer_;
    bool shadowcasting_actiavated_;
    bool lightsystem_activated_;
    World* world_;
};

} // namespace scene

#endif // HORUSEYE_GAME_SCENES_LIGHTRENDERING_H_
