#ifndef HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_
#define HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_

#include "timehandler.h"

namespace framework {

class TimeAccumulator {
  public:
    // Construtores e destrutores
    TimeAccumulator(int duration_miliseconds);
    ~TimeAccumulator() { }

    // Ignora tempo pausado
    int TimeLeft();

    // True se o timer ja expirou
    bool Expired();

    // Despausa se pausado
    void Restart();

    // Despausa se pausado e ataliza a duracao
    void Restart(int duration_miliseconds);

    // Pausa o timer, desconsiderando o tempo ate que seja despausado
    void Pause();

    // Despausa o contador
    void Resume();

    // True se pausado. False caso contrario
    bool IsPaused();

  private:
    TimeHandler *handler_;
    uint32 initial_time_, time_paused_, when_paused_, duration_;
};

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_
