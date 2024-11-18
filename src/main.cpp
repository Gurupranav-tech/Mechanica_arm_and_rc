#include <Arduino.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "utils.hpp"

// #define PS3_READYLED 17
#define MODE_BTN 34
// #define MODE_LED 19
// #define SERVO_DELAY 50
// #define CLAW_SERVO 12
// #define SPIN_SERVO 14

Mode mode = Mode::LINE;
// Servo clawServo, spinServo;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(MODE_BTN, INPUT_PULLDOWN);

    Serial.begin(9600);
    Ps3.begin("7c:2a:31:c2:06:09");
    while (!Ps3.isConnected())
    {
        Serial.print('.');
    }
    digitalWrite(LED_BUILTIN, HIGH);
    Wire.begin();

    // ESP32PWM::allocateTimer(0);
    // ESP32PWM::allocateTimer(1);
    // ESP32PWM::allocateTimer(2);
    // ESP32PWM::allocateTimer(3);
    // clawServo.setPeriodHertz(50);
    // clawServo.attach(CLAW_SERVO, 1000, 2000);
    // spinServo.setPeriodHertz(50);
    // spinServo.attach(SPIN_SERVO, 1000, 2000);
    // clawServo.write(0);
    // spinServo.write(0);
}

void loop()
{
    if (digitalRead(MODE_BTN))
    {
        mode = Mode::LINE;
    }
    else
    {
        mode = Mode::RC;
    }

    if (mode == Mode::RC)
    {
        Wire.beginTransmission(1);
        // First Mode
        Wire.write(modeToNum(mode));
        // Then data
        Wire.write(Ps3.data.analog.stick.lx);
        Wire.write(Ps3.data.analog.stick.lx <= 0 ? 0 : 1);
        Wire.write(Ps3.data.analog.stick.ly);
        Wire.write(Ps3.data.analog.stick.ly <= 0 ? 0 : 1);
        Wire.endTransmission();
    }

    // if (Ps3.data.button.cross)
    // {
    //     claw_open += 10;
    //     if (claw_open >= 180)
    //         claw_open = 180;
    //     clawServo.write(claw_open);
    //     delay(SERVO_DELAY);
    // }
    // if (Ps3.data.button.triangle)
    // {
    //     claw_open -= 10;
    //     if (claw_open <= 0)
    //         claw_open = 0;
    //     clawServo.write(claw_open);
    //     delay(SERVO_DELAY);
    // }
    // if (Ps3.data.button.r1)
    // {
    //     spin_servo += 10;
    //     if (spin_servo >= 180)
    //         spin_servo = 180;
    //     spinServo.write(spin_servo);
    //     delay(SERVO_DELAY);
    // }
    // if (Ps3.data.button.l1)
    // {
    //     spin_servo -= 10;
    //     if (spin_servo <= 0)
    //         spin_servo = 0;
    //     spinServo.write(spin_servo);
    //     delay(SERVO_DELAY);
    // }
}
