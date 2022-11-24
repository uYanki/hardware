#include <Arduino.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.println(analogRead(14));
    delay(20);
}
