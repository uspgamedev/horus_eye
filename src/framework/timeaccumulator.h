//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/timeaccumulator.h
// Definicao da classe TimeAccumulator
//
#ifndef HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_
#define HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_

namespace framework {

class TimeAccumulator {
  public:
    // Construtores e destrutores
    TimeAccumulator(TimeHandler handler, int duration);
    ~TimeAccumulator() { }

    /* Ignora tempo pausado. */
    Uint32 TimeLeft();

    /* True se o timer ja expirou. */
    bool Expired();

    /* Despausa se pausado. */
    void Restart();

    /* Despausa se pausado e ataliza a duracao. */
    void Restart(int duration);

    /* Pausa o timer, desconsiderando o tempo ate que seja despausado. */
    void Pause();

    /* Despausa o contador. */
    void Resume();

    /* True se pausado. False caso contrario. */
    bool IsPaused();

  private:
    TimeHandler handler_;
    Uint32 initial_time_, time_paused_, when_paused_, duration_;
};

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_
