#include <ZMPT101B.h>
ZMPT101B sensor(A0);
const float SENSITIVITY = 0.001475;  // Пример: значение, вычисленное при калибровке

void setup() {
  Serial.begin(9600);
  analogReadResolution(12);
  sensor.setSensitivity(SENSITIVITY);
}

void loop() {
  float voltage = sensor.getVoltage(50, 5);
  int rounded = (int)(voltage + 0.5);  // Округляем до целого
  Serial.print("Voltage: ");
  Serial.print(rounded);
  Serial.println(" V");
  delay(1000);
}
