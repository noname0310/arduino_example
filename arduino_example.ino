#include <Servo.h>
#define SERVO_PIN 10

Servo servo;

void setup() {
    servo.attach(SERVO_PIN);
    servo.write(90);
    delay(1000);
}

void loop() {
    servo.write(0);
    delay(1000);
    servo.write(90);
    delay(1000);
    servo.write(180);
    delay(1000);
}
