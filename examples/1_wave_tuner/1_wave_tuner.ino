#include <ZMPT101B.h>
ZMPT101B zmpt(A0);

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // 12-битный АЦП:contentReference[oaicite:6]{index=6}
}

void loop() {
  int raw = analogRead(A0);
  Serial.println(raw);
  delayMicroseconds(1000);
}
