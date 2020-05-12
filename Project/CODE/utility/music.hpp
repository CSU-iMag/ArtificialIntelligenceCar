#ifndef MUSIC_H
#define MUSIC_H

#include "timer.hpp"
#include <utility>
#include <vector>

enum music_freq {
    MUSIC_DO = 1046,
    MUSIC_RE = 1175,
    MUSIC_MI = 1319,
    MUSIC_FA = 1397,
    MUSIC_SOL = 1568,
    MUSIC_LA = 1760,
    MUSIC_SI = 1976
};

/**
 * @brief 乐谱
 * @note first频率，second时长
 */
typedef std::vector<std::pair<uint16_t, uint16_t>> music_t;

struct MusicPlayer {
    MusicPlayer(const music_t &Music)
        : tmr(std::bind(&MusicPlayer::Play, this, std::placeholders::_1)),
          music(Music), ptr(0) {}
    void Start();

  private:
    const unsigned tim = 800;
    const music_t &music;
    size_t ptr;
    SoftTimer tmr;
    void Play(sched_event_data_t);
};

extern MusicPlayer SongOfJoy;

#endif
