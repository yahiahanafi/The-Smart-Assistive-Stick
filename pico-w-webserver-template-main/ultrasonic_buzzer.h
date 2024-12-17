#ifndef ULTRASONIC_BUZZER_H
#define ULTRASONIC_BUZZER_H

#include "pico/stdlib.h"

// Pin definitions
#define TRIG_PIN 15
#define ECHO_PIN 14
#define BUZZER_PIN 19


// Distance threshold in cm
#define DISTANCE_THRESHOLD 15.0

// Function declarations
void ultrasonic_buzzer_init(void);
float measure_distance(void);
void check_and_trigger_buzzer(float distance);

#endif // ULTRASONIC_BUZZER_H
