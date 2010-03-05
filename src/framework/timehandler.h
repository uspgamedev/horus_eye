//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/timehandler.h
// Definicao da classe TimeHandler.
//
#ifndef HORUSEYE_FRAMEWORK_TIMEHANDLER_H_
#define HORUSEYE_FRAMEWORK_TIMEHANDLER_H_

namespace framework {

// TODO(HenriqueG): descricao de como a classe TimeHandler foi(sera) implementada
class TimeHandler {
  public:
    // Construtores e destrutores
    TimeHandler();
    ~TimeHandler() { }

    // Informa ao TimeHandler que uma nova iteracao iniciou
    void Update();

    // Devolve quanto tempo se passou desde a inicializacao, em milisegundos
    Uint32 TimeElapsed();

    // Devolve quanto tempo se passou desde a iteracao anterior, em milisegundos
    Uint32 TimeDifference();

    // Devolve quanto tempo se passou desde um ponto no tempo
    Uint32 TimeSince(Uint32 t0);

    void Pause();
    void Resume();
    bool IsPaused();

  private:
    Uint32 initial_time_, current_time_, last_update_, time_paused_;
    bool paused_;
};

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_TIMEHANDLER_H_
