#ifndef HORUSEYE_GAME_CAMPAIGNS_EXCEPTIONS_H_
#define HORUSEYE_GAME_CAMPAIGNS_EXCEPTIONS_H_

#include <ugdk/system/exceptions.h>

namespace campaigns {

class AnotherCampaignExists : public ugdk::system::BaseException {
public:
    AnotherCampaignExists() : ugdk::system::BaseException("Attempting to create a new Campaign while another exists.") {}
};

class MissingDescriptorField : public ugdk::system::BaseException {
public:
    MissingDescriptorField(const std::string& field)
        : ugdk::system::BaseException(("Missing field: " + field).c_str()) {}
};

} // namespace campaigns

#endif // HORUSEYE_GAME_CAMPAIGNS_EXCEPTIONS_H_