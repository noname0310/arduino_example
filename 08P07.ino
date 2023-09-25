// Arduino pin assignment
#define PIN_LED 9
#define PIN_TRIG 12 // sonar sensor TRIGGER
#define PIN_ECHO 13 // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define DIST_MIN 100.0    // minimum distance to be measured (unit: mm)
#define DIST_MAX 300.0    // maximum distance to be measured (unit: mm)
#define TARGET_DISTANCE 200.0 // target distance (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define DURATION2DISTANCE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

unsigned long last_sampling_time; // unit: msec

void setup() {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_TRIG, OUTPUT); // sonar TRIGGER
    pinMode(PIN_ECHO, INPUT); // sonar ECHO
    digitalWrite(PIN_TRIG, LOW); // turn-off Sonar

    Serial.begin(57600);
}

void loop() {
    if (millis() < (last_sampling_time + INTERVAL)) return;

    float distance = measure_from_uss(PIN_TRIG, PIN_ECHO);

    if ((distance == 0.0) || (distance > DIST_MAX)) distance = DIST_MAX + 10.0;
    else if (distance < DIST_MIN) distance = DIST_MIN - 10.0;

    float distance_from_target = distance;
    if (distance_from_target < DIST_MIN) distance_from_target = DIST_MIN;
    else if (distance_from_target > DIST_MAX) distance_from_target = DIST_MAX;
    distance_from_target -= TARGET_DISTANCE;
    distance_from_target = abs(distance_from_target);

    int inv_brightness = distance_from_target * 255 / ((DIST_MAX - DIST_MIN) / 2);
    analogWrite(PIN_LED, inv_brightness);

    Serial.print("Min:");
    Serial.print(DIST_MIN);
    Serial.print(",distance:");
    Serial.print(distance);
    Serial.print(",Max:");
    Serial.print(DIST_MAX);
    Serial.println("");

    last_sampling_time = millis();
}

// get a distance reading from USS. return value is in millimeter.
float measure_from_uss(int trigger, int echo) {
    digitalWrite(trigger, HIGH);
    delayMicroseconds(PULSE_DURATION);
    digitalWrite(trigger, LOW);

    return pulseIn(echo, HIGH, TIMEOUT) * DURATION2DISTANCE; // unit: mm
}
