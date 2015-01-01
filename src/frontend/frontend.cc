
#include "frontend/frontend.h"

#include "frontend/nativebuilders.h"

#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>

namespace frontend {

void Initialize() {
    ugdk::system::PushSceneFactory(nativebuilders::HomeScene);
}

} // namespace frontend