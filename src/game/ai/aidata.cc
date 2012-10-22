#include <cstdlib>
#include "game/ai/aidata.h"

namespace ai {

AIData::~AIData() {
}

void AIData::SetSharedData(const std::string& key, ugdk::script::VirtualObj& value) {
    shared_data_[key] = value;
}

ugdk::script::VirtualObj AIData::GetSharedData(const std::string& key) {
    if (shared_data_.count(key))    return shared_data_[key];
    return ugdk::script::VirtualObj();
}

void AIData::ClearSharedData(const std::string& key) {
    if (shared_data_.count(key))    shared_data_.erase(key);
}

void AIData::Clear() {
    direction_ = ugdk::Vector2D();
    aim_destination_ = ugdk::Vector2D();
    using_skills_.clear();
}

}
