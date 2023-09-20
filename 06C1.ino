constexpr int PIN_LED = 7;


int high_time = 1000;
int low_time = 0;

void set_period(int period) { // period: 100-10000 (unit: us)
    high_time = period;
    low_time = 0;
}

void set_duty(int duty) { // duty: 0-100 (unit: %)
    int period = high_time + low_time;

    high_time = period * duty / 100;
    low_time = period - high_time;
}


unsigned long last_time = 0;

unsigned long delta_time() {
    unsigned long now = micros();
    unsigned long delta = now - last_time;
    last_time = now;
    return delta;
}

int brightness = 0;
int direction = 1;

unsigned long time_accum = 0;

constexpr unsigned long blink_period = 1000000; // 1sec

void setup() {
    pinMode(PIN_LED, OUTPUT);

    set_period(100); // 0.1ms
    // set_period(1000); // 1ms
    // set_period(10000); // 10ms
}

void loop() {
    set_duty(brightness);

    digitalWrite(PIN_LED, HIGH);
    delayMicroseconds(high_time);

    digitalWrite(PIN_LED, LOW);
    delayMicroseconds(low_time);

    time_accum += delta_time();

    if (time_accum >= blink_period / (100 * 2)) { // divide 1sec into 200 steps
        time_accum = 0;

        brightness += direction;
        if (brightness == 100) {
            direction = -1;
        }

        if (brightness == 0) direction = 1;
    }
}
