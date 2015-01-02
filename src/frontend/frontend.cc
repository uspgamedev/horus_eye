
#include "frontend/frontend.h"

#include "frontend/nativebuilders.h"
#include "frontend/debugtools.h"
#include "game/campaigns/campaign.h"
#include "game/core/world.h"
#include "game/utils/settings.h"
#include "game/config.h"

#include <ugdk/action/scene.h>
#include <ugdk/system/engine.h>
#include <ugdk/text/module.h>
#include <ugdk/input/events.h>

namespace frontend {

void PopuplateUGDKConfiguration(ugdk::system::Configuration& config) {
    auto settings = utils::Settings::reference();
    config.canvas_size = settings->resolution_vector();
    config.windows_list[0].title = "Horus Eye";
    config.windows_list[0].size = settings->resolution_vector();
    config.windows_list[0].fullscreen = settings->fullscreen();
}

void Initialize() {
    ugdk::text::manager()->RegisterLanguage("en_US", "text/lang_en.txt");
    ugdk::text::manager()->RegisterLanguage("pt_BR", "text/lang_pt_br.txt");
    ugdk::text::manager()->Setup(utils::Settings::reference()->language_name());

#ifdef HORUSEYE_DEBUG_TOOLS
    ugdk::system::GlobalEventHandler().AddObjectListener(DebugToolsListener());
#endif
}

void Start() {
    ugdk::system::PushSceneFactory(nativebuilders::HomeScene);
}

} // namespace frontend