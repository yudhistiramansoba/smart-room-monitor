#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

void mulaiSensor();
void bacaSensor();

extern float suhu;
extern float kelembapan;
extern int cahaya;
extern bool gerakan;

#endif