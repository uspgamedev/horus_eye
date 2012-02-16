#ifndef HORUSEYE_FRAMEWORK_ANIMATIONSET_H_
#define HORUSEYE_FRAMEWORK_ANIMATIONSET_H_

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <ugdk/graphic/modifier.h>
#include <ugdk/action/animation.h>
#include <ugdk/action/animationframe.h>

namespace ugdk {

class AnimationSet {

  public:

    AnimationSet();
    ~AnimationSet();

    // Deletes all this AnimationSet's content from memory.
    // Use with caution.
    void Release();

    // Returns the animation indexed by index, or NULL if it is not there.
    Animation* Get(size_t index);

    // Adds a modifier sequence to the set, naming it for later requests.
    void Add(std::string name, Animation *sequence);
    void Add(std::string name, ...);

    // Searches for the animation using the given name.
    // Returns it if it is found or NULL if else.
    Animation* Search(std::string name);

    // Optimizes access to the animations identified by the given name.
    // The caller should be conscious of the returned indexes for later use
    // of these animations through the Get() method.
    // Returns the generated index or -1 if the animation was not found.
    uint32 MakeIndex(std::string name);

    void Print(FILE *out);

  private:

    typedef std::map<std::string,Animation*> SequenceMap;
    typedef std::vector<Animation*> IndexArray;

    SequenceMap sequences_;
    IndexArray indexed_sequences_;

};

}

#endif /* HORUSEYE_FRAMEWORK_ANIMATIONSET_H_*/
