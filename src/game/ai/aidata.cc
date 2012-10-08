#include <cstdlib>
#include "game/ai/aidata.h"

namespace ai {

AIData::~AIData() {
}

void AIData::SetSharedData(const std::string& key, void* value) {
    shared_data_[key] = value;
}

void* AIData::GetSharedData(const std::string& key) {
    if (shared_data_.count(key))    return shared_data_[key];
    return NULL;
}

void AIData::Clear() {
    direction_ = ugdk::Vector2D();
    aim_destination_ = ugdk::Vector2D();
    using_skills_.clear();
}

}
