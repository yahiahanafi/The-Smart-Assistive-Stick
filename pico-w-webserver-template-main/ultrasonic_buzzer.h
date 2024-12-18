#ifndef ULTRASONIC_BUZZER_H
#define ULTRASONIC_BUZZER_H

#include "pico/stdlib.h"

// Pin definitions
#define TRIG_PIN 0
#define ECHO_PIN 1
#define BUZZER_PIN 16


// Distance threshold in cm
#define DISTANCE_THRESHOLD 15.0

// Function declarations
void ultrasonic_buzzer_init(void);
float measure_distance(void);
void check_and_trigger_buzzer(float distance);

#endif // ULTRASONIC_BUZZER_H
