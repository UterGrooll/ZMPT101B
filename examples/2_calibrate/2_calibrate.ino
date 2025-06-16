#include <ZMPT101B.h>
ZMPT101B sensor(A0);
const float ACTUAL_VOLTAGE = 226.0;  // Фактическое RMS напряжение сети (измеренное вальтметром)

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);
  Serial.println("Calibration...");
}

void loop() {
  // Измеряем выходное RMS напряжение (несколько циклов для повышения точности)
  float measured = sensor.getRMSOutput(50, 3);  
  float sensitivity = measured / ACTUAL_VOLTAGE;
  Serial.print("Measured RMS output (V): ");
  Serial.println(measured, 3);
  Serial.print("Calculated sensitivity: ");
  Serial.println(sensitivity, 6);
  while (1);  // Останавливаемся после одного измерения
}
