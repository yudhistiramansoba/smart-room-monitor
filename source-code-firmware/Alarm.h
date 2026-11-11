#ifndef ALARM_H
#define ALARM_H
extern float BATAS_SUHU;

extern bool buzzerAktif;

#include <Arduino.h>

void mulaiAlarm();
void aturAlarm(float suhu);

#endif