//
// Horus Eye - Framework
// Copyright (C) 2010  Nucleo de Desenvolvimento de Jogos da USP
//
// framework/timehandler.cc
// Implementacao da classe TimeHandler.
//
#include <SDL/SDL.h>
#include <SDL/SDL_timer.h>
#include "timehandler.h"
#include "timeaccumulator.h"

namespace framework {

TimeAccumulator::TimeAccumulator(TimeHandler handler, int duration) {
    handler_ = handler;
    Restart(duration);
}

Uint32 TimeAccumulator::TimeLeft() {
    return duration_ - (handler_.TimeSince(initial_time_) - time_paused_);
}

bool TimeAccumulator::Expired() {
    return TimeLeft() <= 0;
}

/* Despausa se pausado. */
void TimeAccumulator::Restart() {
    Restart(duration_);
}

void TimeAccumulator::Restart(int duration) {
    initial_time_ = handler_.TimeElapsed();
    duration_ = (Uint32) duration;
    time_paused_ = when_paused_ = 0;
}

void TimeAccumulator::Pause() {
    if(when_paused_ == 0)
        when_paused_ = handler_.TimeElapsed();
}

void TimeAccumulator::Resume() {
    if(when_paused_ > 0) {
        time_paused_ += handler_.TimeSince(when_paused_);
        when_paused_ = 0;
    }
}

bool TimeAccumulator::IsPaused() {
    return when_paused_ == 0;
}

}

