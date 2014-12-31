#ifndef HORUSEYE_FRONTEND_NATIVEBUILDERS_H_
#define HORUSEYE_FRONTEND_NATIVEBUILDERS_H_

#include <ugdk/action.h>
#include <memory>

namespace frontend {
namespace nativebuilders {

std::unique_ptr<ugdk::action::Scene> HomeScene();
std::unique_ptr<ugdk::action::Scene> PauseScene();
std::unique_ptr<ugdk::action::Scene> SettingsScene();
std::unique_ptr<ugdk::action::Scene> CampaignSelectionScene();

} // namespace nativebuilders
} // namespace frontend

#endif // HORUSEYE_FRONTEND_NATIVEBUILDERS_H_
