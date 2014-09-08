#include "game/map/giantfloor.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/defaultshaders.h>

#include "game/core/coordinates.h"

#include "game/map/room.h"
#include "game/scenes/world.h"
#include "game/scenes/lightrendering.h"
#include "game/constants.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;
    
namespace {
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };

    ugdk::graphic::opengl::ShaderProgram* continuous_light_shader_ = nullptr;
    void AddHorusLightShader() {
        if (continuous_light_shader_) return;

        opengl::Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        // VERTEX
        vertex_shader.AddCodeBlock("out highp vec2 UV;" "\n"
                                   "out highp vec2 lightUV;" "\n"
                                   "uniform highp vec2 ROOM_POSITION;" "\n"
                                   "uniform highp vec2 HOTSPOT;" "\n"
                                   "uniform highp vec2 LEVEL_SIZE;" "\n"
                                   );
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition,0,1) + vec4(HOTSPOT,0,0);" "\n"
                                    "	UV = vertexUV;" "\n"
                                    "   lightUV = (ROOM_POSITION + vertexUV - (HOTSPOT + vec2(1,1)) * 0.5) / LEVEL_SIZE;"
                                    );
        vertex_shader.GenerateSource();

        // FRAGMENT
        fragment_shader.AddCodeBlock("in highp vec2 UV;" "\n"
                                     "in highp vec2 lightUV;" "\n"
                                     "uniform highp sampler2D drawable_texture;" "\n"
                                     "uniform highp vec4 effect_color;" "\n"
                                     "uniform highp sampler2D light_texture;" "\n");

        fragment_shader.AddLineInMain("	highp vec4 color = texture2D( drawable_texture, UV ) * effect_color;" "\n");
        fragment_shader.AddLineInMain("	color *= vec4(texture2D(light_texture, lightUV).rgb, 1.0);" "\n");
        fragment_shader.AddLineInMain(" gl_FragColor = color;" "\n");
        fragment_shader.GenerateSource();


        continuous_light_shader_ = new opengl::ShaderProgram;

        continuous_light_shader_->AttachShader(vertex_shader);
        continuous_light_shader_->AttachShader(fragment_shader);

        bool status = continuous_light_shader_->SetupProgram();
        assert(status);
    }

}

GiantFloor::GiantFloor(const Room* room)
    : room_(room)
    , size_(106 * room->size().x, 54 * room->size().y)
    , data_(4, sizeof(VertexXYUV), false)
    , texture_(ugdk::resource::GetTextureFromFile("images/ground_texture.png"))
{    
    Vector2D size = room->size();
    AddHorusLightShader();
    {
        VertexData::Mapper mapper(data_);

        {
            VertexXYUV* p = mapper.Get<VertexXYUV>(0);
            std::tie(p->x, p->y) = static_cast<std::pair<double,double>>(core::FromWorldCoordinates({0.0 - 0.5, 0.0 - 0.5}));
            p->u = 0;
            p->v = 0;
        }
        {
            VertexXYUV* p = mapper.Get<VertexXYUV>(1);
            std::tie(p->x, p->y) = static_cast<std::pair<double,double>>(core::FromWorldCoordinates({size.x - 0.5, 0.0 - 0.5}));
            p->u = size.x;
            p->v = 0;
        }
        {
            VertexXYUV* p = mapper.Get<VertexXYUV>(2);
            std::tie(p->x, p->y) = static_cast<std::pair<double,double>>(core::FromWorldCoordinates({0.0 - 0.5, size.y - 0.5}));
            p->u = 0;
            p->v = size.y;
        }
        {
            VertexXYUV* p = mapper.Get<VertexXYUV>(3);
            std::tie(p->x, p->y) = static_cast<std::pair<double,double>>(core::FromWorldCoordinates({size.x - 0.5, size.y - 0.5}));
            p->u = size.x;
            p->v = size.y;
        }

        Vector2D room_pos = core::FromWorldCoordinates(room->position());
        for (size_t i = 0; i < data_.num_vertices(); ++i) {
            VertexXYUV* p = mapper.Get<VertexXYUV>(i);
            p->x += room_pos.x;
            p->y += room_pos.y;
        }

    }
}

GiantFloor::~GiantFloor() {}

void GiantFloor::Draw(ugdk::graphic::Canvas& canvas, const ugdk::graphic::TextureUnit& light_unit) const {

    const glm::mat4& mat = canvas.current_geometry().AsMat4();
    glm::vec4 transformed_hotspot = mat * glm::vec4(-hotspot_.x, -hotspot_.y, 0, 0);

    // Use our shader
    canvas.ChangeShaderProgram(continuous_light_shader_);
    canvas.SendUniform("HOTSPOT", transformed_hotspot.x, transformed_hotspot.y);
    canvas.SendUniform("ROOM_POSITION", room_->position().x, room_->position().y);
    canvas.SendUniform("LEVEL_SIZE", room_->level()->size().x, room_->level()->size().y);

    auto unit = ugdk::graphic::manager()->ReserveTextureUnit(texture_);
    canvas.SendUniform("drawable_texture", unit);
    canvas.SendUniform("light_texture", light_unit);

    canvas.SendVertexData(data_, ugdk::graphic::VertexType::VERTEX, 0, 2);
    canvas.SendVertexData(data_, ugdk::graphic::VertexType::TEXTURE, 2 * sizeof(GLfloat), 2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

} // namespace map
