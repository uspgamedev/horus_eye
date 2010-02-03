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
class TimeHandler
{
  public:
    // Construtores e destrutores
    TimeHandler() { 
        initial_time = SDL_GetTicks();
    }
    ~TimeHandler() { }
    
    // TODO(HenriqueG): implementar todas as seguintes funcoes
    void Update();
    Uint32 ElapsedTime();
    Uint32 TimeDifference();
    Uint32 TimeSince(Uint32 t0);
    void Pause();
    void Resume();
    bool IsPaused();
    
   private:
    Uint32 initial_time, last_update;
}

}  // namespace framework

#endif  // HORUSEYE_FRAMEWORK_TIMEHANDLER_H_
