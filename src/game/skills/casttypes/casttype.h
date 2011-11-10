#ifndef HORUSEYE_GAME_SKILLS_CASTTYPES_CASTTYPE_H_
#define HORUSEYE_GAME_SKILLS_CASTTYPES_CASTTYPE_H_

namespace skills {
namespace casttypes {

/**
 *  This class is merely a formalism for the cast types.
 *  Each cast type should implement the UsageData() function,
 *  which returns the data needed for the Use() function of
 *  a given skill.
 */
template<class T>
class CastType<T> {
  public:
    /**
     *  Returns the data that the Use() function of a given skill which
     *  a specific cast type needs in order to work.
     */
    const T& UsageData() = 0;
};

} // casttypes
} // skills

#endif