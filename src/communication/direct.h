#ifndef HORUSEYE_COMMUNICATION_DIRECT_H_
#define HORUSEYE_COMMUNICATION_DIRECT_H_

#include <string>

namespace communication {
namespace notify {
    void CampaignLevelLoaded();
    void CampaignFailedToLoadLevel(const std::string&);
    void PlaySound(const std::string&);
    void ChangeMusic(const std::string&);
}
} // namespace communication

#endif // HORUSEYE_COMMUNICATION_DIRECT_H_
