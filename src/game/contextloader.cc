#include "game/context.h"

#include <ugdk/resource/module.h>

namespace context {

void LoadTextureAtlas(const std::string& tag, const std::string& filepath) {
    ugdk::resource::manager()->get_container<ugdk::graphic::TextureAtlas*>()->Load(filepath, tag);
}

void LoadTextureAtlas(const std::string& tag, ugdk::graphic::TextureAtlas* atlas) {
    ugdk::resource::manager()->get_container<ugdk::graphic::TextureAtlas*>()->Insert(tag, atlas);
}

} // namespace context