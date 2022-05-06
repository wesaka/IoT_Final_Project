/*
 * Arduino code for the IoT Project
 * Automatic Plant Waterer - Smart Planter
 *
 * Author: Alvacir Wesley Kalatai Alberti
 */

#include <Arduino.h>

// Defining the pins

#define MOISTURE_SENSOR A0
#define DISTANCE_SENSOR A1
#define IN1 6
#define IN2 7

// Defining the values for the program to run with

#define WATER_LEVEL_LIMIT 650
#define MOISTURE_LIMIT 800
#define CHECKING_INTERVAL 600000

unsigned long previousTime = millis();

void setup() {
    // write your initialization code here
    Serial.begin(9600);
    pinMode(MOISTURE_SENSOR, INPUT);
    pinMode(DISTANCE_SENSOR, INPUT);

    // IN1 and IN2 are for controlling the H-Bridge
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    // Both pins LOW - motor off
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
    //Checks every 10 minutes - 600,000 milliseconds
    unsigned long currentTime = millis();
    if ((unsigned long)(currentTime - previousTime) >= CHECKING_INTERVAL) {
        Serial.print("Moisture Sensor: ");
        Serial.println(analogRead(MOISTURE_SENSOR));

        Serial.print("Distance Sensor: ");
        Serial.println(analogRead(DISTANCE_SENSOR));

        // Checks the water level sensor
        // Will not do anything if water sensor is too low (higher value)
        if (analogRead(DISTANCE_SENSOR) > WATER_LEVEL_LIMIT) {
            // Will keep blinking every 100ms the onboard Arduino led to get the user's attention
            digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            delay(100);
            return;
        }

        // Checks the moisture sensor, if it is higher, the soil is drier
        if (analogRead(MOISTURE_SENSOR) > MOISTURE_LIMIT) {
            // Keep the motor on for 750ms - enough for the pump to work and get a bit of water
            // As we are checking every 10 minutes, it will activate until the soil is wet enough
            // for it not to activate anymore
            digitalWrite(IN1, HIGH);
            delay(750);
            digitalWrite(IN1, LOW);
        }

        // Reset the previous time variable in case we are at the 10 minutes mark
        previousTime = currentTime;
    }
}