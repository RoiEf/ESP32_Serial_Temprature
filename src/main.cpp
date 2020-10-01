#include <Adafruit_MLX90614.h>
#include <Arduino.h>
#include <SerialCommand.h>
#include <Wire.h>

#include <dwd.hpp>

#define BAUD 115200
#define BUZZER 2
#define BUTTON 4
#define DEBAUNCE_DELAY 10  // 10 Ms.

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
double g_Temprature = 0;
bool measured = false;

SerialCommand sCmd;

TIMER btnDebaunce(DEBAUNCE_DELAY);
bool btnState = 0;
bool btnLastState = 0;
bool btnCurrentState;

// Global function declerations
void getTemp(void);
void unrecognized(const char *command);
void beep(int duration);
void beepOL(void);
void measureTemp(void);

void setup() {
    Serial.begin(BAUD);
    mlx.begin();
    // Serial command function callbacks
    sCmd.addCommand("GetTemp", getTemp);   // Converts two arguments to integers and echos them back
    sCmd.addCommand("Beep", beepOL);       // Converts two arguments to integers and echos them back
    sCmd.setDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?")

    pinMode(BUTTON, INPUT_PULLDOWN);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, HIGH);

    delay(100);

    Serial.println("ESP32 MLX90614");
}

void loop() {
    btnCurrentState = digitalRead(BUTTON);
    if (btnCurrentState != btnLastState) {
        btnDebaunce.resetTimer();
        // Serial.println("Timer Reset");
    }
    if (btnDebaunce.checkInterval()) {
        if (btnCurrentState != btnState) {
            btnState = btnCurrentState;
            // Serial.println("State change");
        }

        if (btnState) {
            measureTemp();
        } else {
            measured = false;
        }
    }
    btnLastState = btnCurrentState;

    sCmd.readSerial();  // We don't do much, just process serial commands
}

void getTemp(void) {
    Serial.print(g_Temprature);
    Serial.println("*C");
}
// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
    Serial.println("What?");
}

void beep(int duration) {
    digitalWrite(BUZZER, LOW);
    // ledcWrite(channel, 125);
    delay(duration);
    digitalWrite(BUZZER, HIGH);
    // ledcWrite(channel, 0);
}
void beepOL(void) {
    beep(1000);
}
void measureTemp(void) {
    double acc = 0;
    mlx.readAmbientTempC();
    for (int i = 0; i < 20; i++) {
        acc += mlx.readObjectTempC();
        delay(50);
    }
    acc /= 20;
    acc *= 1.05;
    g_Temprature = acc;
    measured = true;
    beep(1000);
}