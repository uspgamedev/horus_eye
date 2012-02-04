#ifndef UGDK_TIME_TIMEMANAGER_H_
#define UGDK_TIME_TIMEMANAGER_H_

#include <ugdk/base/types.h>

namespace ugdk {
namespace time {

class TimeManager {
  public:
    TimeManager();
    ~TimeManager() { }

    /// Informs the TimeManager that a new frame started.
    void Update();

    /*  @return Time elapsed since initialization, in milliseconds */
    uint32 TimeElapsed();

    // Devolve quanto tempo se passou desde a iteracao anterior, em milisegundos
    /*  @return Time elapsed since last frame, in milliseconds */
    uint32 TimeDifference();

    // Devolve quanto tempo se passou desde um ponto no tempo
    uint32 TimeSince(uint32 t0);

  private:
    uint32 initial_time_, current_time_, last_update_;
};

}  // namespace time
}  // namespace ugdk

#endif  // HORUSEYE_FRAMEWORK_TIMEHANDLER_H_
