#include "SoundController.h"



void SoundController::play_ring(unsigned long global_timer)
{
  if(global_timer-ringthone_timer>=ringthone_duration)
  {
    startPlayback(ring_sound, sizeof(ring_sound));
    ringthone_timer=global_timer;
  }
}

void SoundController::play_silence(unsigned long global_timer)
{
  if(global_timer-silencer_timer>=silencer_gap && sim_status!=INCOMMING)
  {
    startPlayback({}, sizeof({}));
    silencer_timer=global_timer;
  }
}