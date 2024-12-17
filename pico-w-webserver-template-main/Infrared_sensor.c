#include <stdio.h>
#include "pico/stdlib.h"
#include "infrared_buzzer.h"

// Initialize GPIO pins for the IR sensor and buzzer
void infrared_buzzer_init(void) {
    gpio_init(IR_SENSOR_PIN);
    gpio_init(VIBRATE_PIN);

    gpio_set_dir(IR_SENSOR_PIN, GPIO_IN);  // IR sensor as input
    gpio_set_dir(VIBRATE_PIN, GPIO_OUT);   // Buzzer as output
}

// Function to check the IR sensor and control the buzzer
void check_and_trigger_ir_buzzer(void) {
    // Read the IR sensor output
    bool object_detected = !gpio_get(IR_SENSOR_PIN);
    // Many IR sensors output LOW (0) when an object is detected

    if (object_detected) {
        printf("Object Detected!\n");
        gpio_put(VIBRATE_PIN, 0); // Turn on the buzzer
    } else {
        printf("No Object Detected\n");
        gpio_put(VIBRATE_PIN, 1); // Turn off the buzzer
    }
}
