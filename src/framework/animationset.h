#ifndef HORUSEYE_FRAMEWORK_ANIMATIONSET_H_
#define HORUSEYE_FRAMEWORK_ANIMATIONSET_H_

#include <string>
#include <map>
#include <vector>
#include <utility>

namespace framework {

class Animation;

class AnimationSet {

  public:

    AnimationSet(size_t size);
    virtual ~AnimationSet();

    // Returns the animation indexed by index, or NULL if it is not there.
    Animation* Get(int index);

    // Adds a animation to the set, naming it for later requests.
    void Add(std::string name, Animation* animation);

    // Searches for the animation using the given name.
    // Returns it if it is found or NULL if else.
    Animation* Search(std::string name);

    // Uses the given index to optimize access to the animations identified by
    // the given name.
    // The caller should be counscious of the given indexes for later use
    // of these animations through the Get() method.
    // Returns true if the animation was successfully indexed or false if else.
    bool MakeIndex(int index, std::string name);

  private:

    typedef std::map<std::string,Animation*> AnimationMap;
    typedef std::vector<Animation*> IndexArray;

    size_t size_;
    AnimationMap animations_;
    IndexArray indexed_animations_;

};

}

#endif /* HORUSEYE_FRAMEWORK_ANIMATIONSET_H_*/
