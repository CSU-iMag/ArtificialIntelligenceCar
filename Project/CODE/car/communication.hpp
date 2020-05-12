#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "timer.hpp"

extern SoftTimer Send_Direction_PackData_tim;
extern SoftTimer Send_Speed_PackData_tim;
extern SoftTimer Send_AI_PackData_tim;
extern SoftTimer VisualScopeTmr;

void com_init();
void com_startSend();
void com_stopSend();

#endif
