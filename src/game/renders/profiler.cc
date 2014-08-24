#include "game/renders/profiler.h"

#include <ugdk/system/engine.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/text/textmanager.h>
#include <ugdk/graphic/text/label.h>
#include <ugdk/graphic/text/textbox.h>
#include <ugdk/graphic/opengl/shaderprogram.h>
#include <ugdk/graphic/opengl/shader.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/debug/profiler.h>

#include <sstream>

using namespace ugdk;
using namespace ugdk::graphic;
using math::Vector2D;

namespace renders
{

void SectionDataToString(std::stringstream& out, const std::string& indent, const std::shared_ptr<const ugdk::debug::SectionData>& data) {
    out << indent << data->name << " - " << data->duration_ << '\n';
    for(const auto& subdata : data->subdata)
        SectionDataToString(out, indent + "  ", subdata);
}

void DrawSectionData(ugdk::graphic::Canvas& canvas, const std::shared_ptr<const ugdk::debug::SectionData>& data) {

    double screen_width = graphic::manager()->screen()->size().x;

    std::stringstream msg;
    SectionDataToString(msg, "", data);

    TextBox box(msg.str(), screen_width, TEXT_MANAGER()->current_font());

    box.Draw(canvas);
}

}
