#include "game/context.h"

#include <ugdk/resource/module.h>
#include <ugdk/graphic/textureatlas.h>

namespace context {

void LoadTextureAtlas(const std::string& tag, const std::string& filepath) {
    ugdk::resource::manager()->GetContainer<ugdk::graphic::TextureAtlas>()->Load(filepath, tag);
}

void LoadTextureAtlas(const std::string& tag, ugdk::graphic::TextureAtlas* atlas) {
    ugdk::resource::manager()->GetContainer<ugdk::graphic::TextureAtlas>()->Insert(tag, atlas);
}

} // namespace context
