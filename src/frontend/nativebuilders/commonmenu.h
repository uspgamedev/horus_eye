#ifndef HORUSEYE_FRONTEND_NATIVEBUILDERS_COMMONMENU_H_
#define HORUSEYE_FRONTEND_NATIVEBUILDERS_COMMONMENU_H_

#include <ugdk/action.h>
#include <ugdk/action/events.h>
#include <ugdk/ui.h>
#include <ugdk/ui/hookpoint.h>
#include <ugdk/system/task.h>
#include <ugdk/action.h>

#include <memory>
#include <list>

namespace frontend {
namespace nativebuilders {

void MenuFocus(const ugdk::action::SceneFocusEvent& ev);

void MenuDeFocus(const ugdk::action::SceneDefocusEvent& ev);

void FinishOwner(const ugdk::ui::Button* button);

std::unique_ptr<ugdk::ui::Menu> BaseBuildMenu(ugdk::ui::HookPoint hook = ugdk::ui::HookPoint::CENTER);

class AnimationPlayerHolder : public ugdk::system::Task {
public:
    AnimationPlayerHolder();
    virtual ~AnimationPlayerHolder();

    void Add(const std::shared_ptr<ugdk::action::SpriteAnimationPlayer>& player);

private:
    std::list< std::shared_ptr<ugdk::action::SpriteAnimationPlayer> > players_;
};


} // namespace nativebuilders
} // namespace frontend

#endif // HORUSEYE_FRONTEND_NATIVEBUILDERS_COMMONMENU_H_
