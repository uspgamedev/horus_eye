
#include "animationset.h"

namespace framework {

using namespace std;

AnimationSet::AnimationSet (size_t size = 0) :
    size_(size), indexed_animations_(size, -1) {}

AnimationSet::~AnimationSet () {}

Animation* AnimationSet::Get(int index) {
    if (index >= 0 && index < indexed_animations_.size())
        return indexed_animations_[index];
    else return NULL;
}

void AnimationSet::Add(std::string name, Animation* animation) {
    animations_[name] = animation;
}

Animation* AnimationSet::Search(std::string name) {
    AnimationMap::iterator it = animations_.find(name);
    return it == animations_.end()?
            NULL :
            it->second;
}

bool AnimationSet::MakeIndex(int i, string name) {
    if (i >= 0 && i < this->size_) {
        Animation animation = this->Search(name);
        if (animation == NULL) return false;
        indexed_animations_[i] = animation;
        return true;
    }
    return false;
}

}
