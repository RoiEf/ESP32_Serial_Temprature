#include <Adafruit_MLX90614.h>
#include <Arduino.h>
#include <Wire.h>

#define BAUD 115200

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
void setup() {
    Serial.begin(BAUD);
    mlx.begin();
    delay(100);

    Serial.println("ESP32 MLX90614");
}

void loop() {
    // Serial.print("Ambient = ");
    // Serial.print(mlx.readAmbientTempC());
    // Serial.print("*C\tObject = ");
    // Serial.print(mlx.readObjectTempC());
    // Serial.println("*C");

    // Serial.println();
    delay(50);
}