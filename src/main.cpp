#include <Arduino.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "utils.hpp"

#define PS3_READYLED 17
#define MODE_BTN 34
#define MODE_LED 19
#define SERVO_DELAY 50
#define CLAW_SERVO 12
#define SPIN_SERVO 14
#define X_SERVO 27
#define Y_SERVO 26

Mode mode = Mode::LINE;
Servo clawServo, spinServo, xServo, yServo;

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

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    clawServo.setPeriodHertz(50);
    clawServo.attach(CLAW_SERVO, 1000, 2000);
    spinServo.setPeriodHertz(50);
    spinServo.attach(SPIN_SERVO, 1000, 2000);
    xServo.setPeriodHertz(50);
    xServo.attach(X_SERVO, 1000, 2000);
    yServo.setPeriodHertz(50);
    yServo.attach(Y_SERVO, 1000, 2000);
    clawServo.write(0);
    spinServo.write(0);
}

void loop()
{
    static int claw_open = 0;
    static int spin_servo = 0;
    static int x = 0;
    static int y = 0;

    int status = digitalRead(MODE_BTN);

    if (status)
    {
        mode = Mode::LINE;
    }
    else
    {
        mode = Mode::RC;
    }

    Wire.beginTransmission(1);
    Wire.write(mode == Mode::RC ? 0 : 1);
    if (mode == Mode::RC)
    {
        // First Mode
        // Then data
        Wire.write(Ps3.data.analog.stick.lx);
        Wire.write(Ps3.data.analog.stick.lx <= 0 ? 0 : 1);
        Wire.write(Ps3.data.analog.stick.ly);
        Wire.write(Ps3.data.analog.stick.ly <= 0 ? 0 : 1);
    }
    Wire.endTransmission();

    if (Ps3.data.button.cross)
    {
        claw_open += 10;
        if (claw_open >= 180)
            claw_open = 180;
        clawServo.write(claw_open);
    }
    if (Ps3.data.button.triangle)
    {
        claw_open -= 10;
        if (claw_open <= 0)
            claw_open = 0;
        clawServo.write(claw_open);
    }
    if (Ps3.data.button.r1)
    {
        spin_servo += 10;
        if (spin_servo >= 180)
            spin_servo = 180;
        spinServo.write(spin_servo);
    }
    if (Ps3.data.button.l1)
    {
        spin_servo -= 10;
        if (spin_servo <= 0)
            spin_servo = 0;
        spinServo.write(spin_servo);
    }
    if (Ps3.data.button.up)
    {
        y += 10;
        if (y >= 180)
            y = 180;
        yServo.write(y);
    }
    else if (Ps3.data.button.down)
    {
        y -= 10;
        if (y <= 0)
            y = 0;
        yServo.write(y);
    }
    if (Ps3.data.button.right)
    {
        x += 10;
        if (x >= 180)
            x = 180;
        xServo.write(x);
    }
    else if (Ps3.data.button.left)
    {
        x -= 10;
        if (x <= 0)
            x = 0;
        xServo.write(x);
    }
    delay(SERVO_DELAY);
}
