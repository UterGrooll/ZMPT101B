#include "ZMPT101B.h"

ZMPT101B::ZMPT101B(uint8_t pin) {
  _pin = pin;
  pinMode(_pin, INPUT);
  analogReadResolution(12);     // Установить 12-битный режим АЦП (0–4095):contentReference[oaicite:5]{index=5}
  _Vref = 5.0;                  // Опорное напряжение (в В), по умолчанию 5.0
  _sensitivity = 1.0;           // Чувствительность, по умолчанию 1 (без масштабирования)
  _zeroPoint = 2048;            // Начальный нулевой уровень (половина 4095)
  _filterEnabled = false;
  _filterSize = 0;
  _filterIndex = 0;
  _filterBuffer = nullptr;
}

void ZMPT101B::setVref(float vref) {
  _Vref = vref;
}

void ZMPT101B::setSensitivity(float sens) {
  _sensitivity = sens;
}

void ZMPT101B::setZeroPoint(int zero) {
  _zeroPoint = zero;
}

// Усреднённая калибровка смещения (снимаем offset без входного сигнала)
int ZMPT101B::calibrate(int samples) {
  long sum = 0;
  for(int i = 0; i < samples; i++) {
    sum += analogRead(_pin);
  }
  int avg = sum / samples;
  _zeroPoint = avg;
  return avg;
}

// Настройка фильтра скользящего среднего (0 – выключить)
void ZMPT101B::setFilter(uint8_t windowSize) {
  if (_filterBuffer) {
    delete[] _filterBuffer;
    _filterBuffer = nullptr;
  }
  if (windowSize > 1) {
    _filterEnabled = true;
    _filterSize = windowSize;
    _filterIndex = 0;
    _filterBuffer = new float[_filterSize];
    for (uint8_t i = 0; i < _filterSize; i++) {
      _filterBuffer[i] = 0.0;
    }
  } else {
    _filterEnabled = false;
    _filterSize = 0;
    _filterIndex = 0;
  }
}

// Измерение RMS выходного напряжения (в вольтах) за один или несколько периодов
float ZMPT101B::getRMSOutput(uint16_t freq, uint8_t loops) {
  float totalVrms = 0.0;
  uint32_t period = 1000000UL / freq;  // длина одного периода в микросекундах
  for (uint8_t k = 0; k < loops; k++) {
    uint32_t start = micros();
    uint32_t count = 0;
    double sumSq = 0.0;
    while ((uint32_t)(micros() - start) < period) {
      int reading = analogRead(_pin);
      // Преобразуем показание АЦП в напряжение (с учетом смещения zeroPoint)
      float voltage = ((reading - _zeroPoint) * _Vref) / 4095.0;
      sumSq += (double)voltage * voltage;
      count++;
    }
    float meanSq = sumSq / count;
    float vrms = sqrt(meanSq);         // RMS выходного сигнала (без учета sensitivity)
    totalVrms += vrms;
  }
  return totalVrms / loops;
}

// Получение итогового напряжения с учётом sensitivity. При включённом фильтре – возвращается среднее.
float ZMPT101B::getVoltage(uint16_t freq, uint8_t loops) {
  float vrmsOut = getRMSOutput(freq, loops);
  float vrmsIn = vrmsOut / _sensitivity;  // перевод в реальное напряжение
  if (_filterEnabled && _filterBuffer) {
    _filterBuffer[_filterIndex++] = vrmsIn;
    if (_filterIndex >= _filterSize) _filterIndex = 0;
    float sum = 0.0;
    for (uint8_t i = 0; i < _filterSize; i++) {
      sum += _filterBuffer[i];
    }
    return sum / _filterSize;
  }
  return vrmsIn;
}
