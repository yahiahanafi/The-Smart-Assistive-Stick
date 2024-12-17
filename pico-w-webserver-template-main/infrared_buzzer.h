#ifndef INFRARED_BUZZER_H
#define INFRARED_BUZZER_H

#include "pico/stdlib.h"

// Pin definitions
#define IR_SENSOR_PIN 16
#define VIBRATE_PIN 3

// Function declarations
void infrared_buzzer_init(void);
void check_and_trigger_ir_buzzer(void);

#endif // INFRARED_BUZZER_H
