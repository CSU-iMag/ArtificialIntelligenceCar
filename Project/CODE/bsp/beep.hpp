#ifndef BEEP_H
#define BEEP_H

#include "fsl_qtmr.h"

static const uint16_t beep_musicFreq[7] = {1046, 1175, 1319, 1397,
                                           1568, 1760, 1976};
                                           
//! @note 板载蜂鸣器已在MCUXpressoConfigTool初始化
struct Beep {
    //! @brief set pin mux to qtimer
    void Init();
    //! @brief be quiet
    void Mute();
    //! @brief start beep with Freq
    void BeepFreq(uint32_t Freq);
    //! @brief start beep and stop in time
    void BeepFreqDelay(uint32_t Freq, uint16_t ms);
};

#endif
