#include "game/map/giantfloor.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/shader.h>
#include <ugdk/graphic/shaderprogram.h>
#include <ugdk/graphic/defaultshaders.h>

#include "game/core/coordinates.h"

#include "game/map/room.h"
#include "game/core/world.h"
#include "game/constants.h"
#include "frontend/gameview/lightrendering.h"

namespace map {

using namespace ugdk::graphic;
using ugdk::math::Vector2D;
    
namespace {
    struct VertexXY {
        GLfloat x, y;
    };
    struct VertexXYUV {
        GLfloat x, y, u, v;
    };

    ShaderProgram* continuous_light_shader_ = nullptr;
    void AddHorusLightShader() {
        if (continuous_light_shader_) return;

        Shader vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER);

        // VERTEX
        vertex_shader.AddCodeBlock("out highp vec2 UV;" "\n"
                                   "in highp vec2 inputLightUV;"
                                   "out highp vec2 lightUV;" "\n"
                                   "uniform highp vec2 OFFSET;" "\n"
                                   );
        vertex_shader.AddLineInMain("	gl_Position =  geometry_matrix * vec4(vertexPosition + OFFSET,0,1);" "\n"
                                    "	UV = vertexUV;" "\n"
                                    "   lightUV = inputLightUV;"
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


        continuous_light_shader_ = new ShaderProgram;

        continuous_light_shader_->AttachShader(vertex_shader);
        continuous_light_shader_->AttachShader(fragment_shader);

        bool status = continuous_light_shader_->SetupProgram();
        glBindAttribLocation(continuous_light_shader_->id(), 2, "inputLightUV");
        assert(status);
    }

    void set_vector_fields(VertexXYUV* p, const ugdk::math::Vector2D& vec, double u, double v) {
        p->x = static_cast<GLfloat>(vec.x);
        p->y = static_cast<GLfloat>(vec.y);
        p->u = static_cast<GLfloat>(u);
        p->v = static_cast<GLfloat>(v);
    }
    
    void set_vector_fields(VertexXY* p, const ugdk::math::Vector2D& vec) {
        p->x = static_cast<GLfloat>(vec.x);
        p->y = static_cast<GLfloat>(vec.y);
    }

}

GiantFloor::GiantFloor(const Room* room)
    : room_(room)
    , data_(4, sizeof(VertexXYUV), false)
    , texture_(ugdk::resource::GetTextureFromFile("images/ground_texture.png"))
{    
    Vector2D size = room->size();
    AddHorusLightShader();
    {
        VertexData::Mapper mapper(data_);

        set_vector_fields(mapper.Get<VertexXYUV>(0),
                          core::FromWorldCoordinates({ 0.0 - 0.5, 0.0 - 0.5 }), 0.0, 0.0);
        set_vector_fields(mapper.Get<VertexXYUV>(1),
                          core::FromWorldCoordinates({ size.x - 0.5, 0.0 - 0.5 }), size.x, 0.0);      
        set_vector_fields(mapper.Get<VertexXYUV>(2),
                          core::FromWorldCoordinates({ 0.0 - 0.5, size.y - 0.5 }), 0.0, size.y);             
        set_vector_fields(mapper.Get<VertexXYUV>(3),
                          core::FromWorldCoordinates({ size.x - 0.5, size.y - 0.5 }), size.x, size.y);        
    }
}

GiantFloor::~GiantFloor() {}

void GiantFloor::Draw(ugdk::graphic::Canvas& canvas,
                      const ugdk::graphic::TextureUnit& light_unit,
                      const frontend::gameview::LightRendering& light_rendering) const {

    ugdk::graphic::VertexData lightUV(4, sizeof(VertexXY), false);
    {
        VertexData::Mapper mapper(lightUV);
        Vector2D room_position = Vector2D(room_->position()) - Vector2D(0.5);
        set_vector_fields(mapper.Get<VertexXY>(0), light_rendering.CalculateUV(room_position));
        set_vector_fields(mapper.Get<VertexXY>(1), light_rendering.CalculateUV(room_position + Vector2D(room_->size().x, 0.0)));
        set_vector_fields(mapper.Get<VertexXY>(2), light_rendering.CalculateUV(room_position + Vector2D(0.0, room_->size().y)));
        set_vector_fields(mapper.Get<VertexXY>(3), light_rendering.CalculateUV(room_position + room_->size()));
    }

    // Use our shader
    canvas.ChangeShaderProgram(continuous_light_shader_);
    canvas.SendUniform("OFFSET", core::FromWorldCoordinates(room_->position()));

    auto unit = ugdk::graphic::manager()->ReserveTextureUnit(texture_);
    canvas.SendUniform("drawable_texture", unit);
    canvas.SendUniform("light_texture", light_unit);

    canvas.SendVertexData(data_, ugdk::graphic::VertexType::VERTEX, 0, 2);
    canvas.SendVertexData(data_, ugdk::graphic::VertexType::TEXTURE, 2 * sizeof(GLfloat), 2);
    canvas.SendVertexData(lightUV, ugdk::graphic::VertexType::COLOR, 0, 2);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

} // namespace map
