#ifndef HORUSEYE_FRONTEND_NATIVESCENES_H_
#define HORUSEYE_FRONTEND_NATIVESCENES_H_

#include <ugdk/action.h>
#include <memory>

namespace frontend {
namespace nativescenes {

std::unique_ptr<ugdk::action::Scene> HomeScene();
std::unique_ptr<ugdk::action::Scene> PauseScene();
std::unique_ptr<ugdk::action::Scene> SettingsScene();
std::unique_ptr<ugdk::action::Scene> CampaignSelectionScene();

} // namespace nativescenes
} // namespace frontend

#endif // HORUSEYE_FRONTEND_NATIVESCENES_H_
