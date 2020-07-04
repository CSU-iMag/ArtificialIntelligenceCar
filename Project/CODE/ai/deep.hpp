#ifndef DEEP_H
#define DEEP_H

extern "C" const unsigned char model1[];
extern "C" const unsigned char model2[];
extern "C" const unsigned char model3[];

void deep_init();
float deep_predict();

#endif
