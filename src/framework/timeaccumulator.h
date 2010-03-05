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

// TODO(HenriqueG): descricao de como a classe TimeHandler foi(sera) implementada
class TimeAccumulator {
  public:
    // Construtores e destrutores
    TimeAccumulator(TimeHandler handler, int duration);
    ~TimeAccumulator() { }

    // TODO(HenriqueG): implementar todas as seguintes funcoes
    Uint32 TimeLeft();
    bool Expired();
    void Restart();
    void Restart(int duration);
    void Pause();
    void Resume();
    bool IsPaused();

  private:
    TimeHandler handler_;
    Uint32 initial_time_, time_paused_, when_paused_, duration_;
};

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_TIMEACCUMULATOR_H_
