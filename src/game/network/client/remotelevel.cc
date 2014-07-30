
#include "remotelevel.h"

#include <ugdk/internal/opengl.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/math/vector2D.h>

#include "game/components/graphic.h"

// TODO
#include "game/campaigns/campaign.h"
#include "game/scenes/world.h"

namespace network {

using ugdk::math::Vector2D;

namespace {
    RemoteLevel* THE_REFERENCE_ = nullptr;
}

RemoteLevel::RemoteLevel() {
    THE_REFERENCE_ = this;
    this->set_render_function([this](ugdk::graphic::Canvas& canvas) {
        glEnable(GL_DEPTH_TEST);

        using namespace ugdk::graphic;
        canvas.PushAndCompose(campaigns::Campaign::CurrentCampaign()->current_level()->camera());

        const Geometry& geo = canvas.current_geometry();

        const opengl::ShaderProgram* current_shader = nullptr;
        const ugdk::internal::GLTexture* current_texture = nullptr;
        std::unique_ptr<opengl::ShaderUse> shader_use;

        int shader_changes = 0;
        int texture_changes = 0;
        for (auto graphic : this->graphic_components_) {
            const auto& primitive = graphic->primitive();

            if (primitive.shader_program() != current_shader) {
                shader_use = nullptr;
                shader_use.reset(new opengl::ShaderUse(current_shader = primitive.shader_program()));
                shader_use->SendGeometry(geo);
                shader_changes++;
            }

            if (primitive.texture() != current_texture) {
                shader_use->SendTexture(0, current_texture = primitive.texture());
                texture_changes++;
            }

            glm::vec4 position_ogl = geo.AsMat4() * glm::vec4(graphic->final_position().x, graphic->final_position().y, 0.0, 0.0);
            glm::vec4 render_off_ogl = geo.AsMat4() * glm::vec4(graphic->render_offset().x, graphic->render_offset().y, 0.0, 0.0);
            Vector2D lightpos = (Vector2D(position_ogl.x, position_ogl.y) + geo.offset() - Vector2D(render_off_ogl.x, render_off_ogl.y))* 0.5 + Vector2D(0.5, 0.5);
            shader_use->SendUniform("lightUV", lightpos.x, lightpos.y);

            shader_use->SendEffect(canvas.current_visualeffect() * primitive.visual_effect());
            primitive.drawfunction()(primitive, *shader_use);
        }

        glDisable(GL_DEPTH_TEST);

        canvas.PopGeometry();
    });
}
    
RemoteLevel* RemoteLevel::reference() {
    return THE_REFERENCE_;
}

void RemoteLevel::AddGraphic(component::Graphic* g) {
    this->graphic_components_.push_back(g);
}

void RemoteLevel::RemoveGraphic(component::Graphic* g) {
    this->graphic_components_.remove(g);
}

} // namespace network
