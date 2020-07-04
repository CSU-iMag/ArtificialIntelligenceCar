#include "music.hpp"
#include "car.hpp"

#ifdef BEEP_ENABLED

#define MUSIC_BUZZER Car.beep0

const static music_t SoJ{
    {MUSIC_MI, 500},  {MUSIC_MI, 500}, {MUSIC_FA, 500}, {MUSIC_SOL, 500},
    {MUSIC_SOL, 500}, {MUSIC_FA, 500}, {MUSIC_MI, 500}, {MUSIC_RE, 500},
    {MUSIC_DO, 500},  {MUSIC_DO, 500}, {MUSIC_RE, 500}, {MUSIC_MI, 500},
    {MUSIC_MI, 750},  {MUSIC_RE, 250}, {MUSIC_RE, 790}, {MUSIC_RE, 500},
    {MUSIC_MI, 500},  {MUSIC_MI, 500}, {MUSIC_FA, 500}, {MUSIC_SOL, 500},
    {MUSIC_SOL, 500}, {MUSIC_FA, 500}, {MUSIC_MI, 500}, {MUSIC_RE, 500},
    {MUSIC_DO, 500},  {MUSIC_DO, 500}, {MUSIC_RE, 500}, {MUSIC_MI, 500},
    {MUSIC_RE, 750},  {MUSIC_DO, 250}, {MUSIC_DO, 790}, {MUSIC_DO, 500},
}; //!< 《欢乐颂》

MusicPlayer SongOfJoy(SoJ);

void MusicPlayer::Play(sched_event_data_t dat) {
    MUSIC_BUZZER.BeepFreqDelay(music[ptr].first, music[ptr].second);
    ++ptr %= music.size();
}

void MusicPlayer::Start() { tmr.Start(tim); }

#endif
