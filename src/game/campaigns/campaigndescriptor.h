#ifndef HORUSEYE_GAME_CAMPAIGNS_CAMPAIGNDESCRIPTOR_H_
#define HORUSEYE_GAME_CAMPAIGNS_CAMPAIGNDESCRIPTOR_H_

#include <string>
#include <vector>

namespace campaigns {

class CampaignDescriptor {
public:
    CampaignDescriptor(const std::string& path);

    bool playable() const { return playable_; }
    const std::string& name() const { return name_; }
    const std::string& path() const { return path_; }
    const std::string& script_path() const { return script_path_; }

private:
    bool playable_;
    std::string name_;
    std::string path_;
    std::string script_path_;
};

std::vector<CampaignDescriptor> FindAllCampaignDescriptorsAt(const std::string& path);

} // namespace campaigns

#endif // HORUSEYE_GAME_CAMPAIGNS_CAMPAIGNDESCRIPTOR_H_