#ifndef ZMPT101B_H
#define ZMPT101B_H

#include <Arduino.h>

class ZMPT101B {
public:
  ZMPT101B(uint8_t pin);                // Конструктор с указанием аналогового пина (A0–A3)
  void setVref(float vref);             // Установить опорное напряжение (например, 3.3 или 5.0 В)
  void setSensitivity(float sens);      // Установить чувствительность (outputRMS/inputRMS)
  void setZeroPoint(int zero);          // Явно задать нулевой отсчёт (смещение) в единицах ADC
  int  calibrate(int samples = 1000);   // Автоматическая калибровка смещения (усреднение в указанном кол-ве отсчётов)
  void setFilter(uint8_t windowSize);   // Включить фильтр скользящего среднего (0 – фильтр выключен)
  float getVoltage(uint16_t freq = 50, uint8_t loops = 1);   // Вернуть RMS напряжение (учитывая sensitivity)
  float getRMSOutput(uint16_t freq = 50, uint8_t loops = 1); // Вернуть RMS выходное напряжение (до учёта sensitivity)

private:
  uint8_t _pin;
  float   _Vref;
  float   _sensitivity;
  int     _zeroPoint;
  bool    _filterEnabled;
  uint8_t _filterSize;
  uint8_t _filterIndex;
  float*  _filterBuffer;
};

#endif
