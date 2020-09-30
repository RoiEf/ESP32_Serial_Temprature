#include <Adafruit_MLX90614.h>
#include <Arduino.h>
#include <SerialCommand.h>
#include <Wire.h>

#define BAUD 115200

// #define BUZZER 15
// #define freq 2000
// #define channel 0
// #define resolution 8

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
SerialCommand sCmd;

// Global function declerations
void getTemp(void);
void unrecognized(const char *command);
// void beep(int duration);

void setup() {
    Serial.begin(BAUD);
    mlx.begin();
    // Serial command function callbacks
    sCmd.addCommand("GetTemp", getTemp);   // Converts two arguments to integers and echos them back
    sCmd.setDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")

    // set PWM for buzzer
    // ledcSetup(channel, freq, resolution);
    // ledcAttachPin(BUZZER, channel);

    delay(100);

    Serial.println("ESP32 MLX90614");
}

void loop() {
    sCmd.readSerial();  // We don't do much, just process serial commands
    delay(5);
}

void getTemp(void) {
    double acc = 0;
    mlx.readAmbientTempC();
    for (int i = 0; i < 20; i++) {
        acc += mlx.readObjectTempC();
        delay(50);
    }
    acc /= 20;
    Serial.print(acc);
    Serial.println("*C");
}
// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
    Serial.println("What?");
}

// void beep(int duration) {
// }