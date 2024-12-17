#include <stdio.h>
#include "pico/stdlib.h"
#include "ultrasonic_buzzer.h"

// Initialize GPIO pins for the ultrasonic sensor and buzzer
void ultrasonic_buzzer_init(void) {
    gpio_init(TRIG_PIN);
    gpio_init(ECHO_PIN);
    gpio_init(BUZZER_PIN);

    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

// Function to measure distance using the ultrasonic sensor
float measure_distance(void) {
    // Ensure TRIG is low before starting
    gpio_put(TRIG_PIN, 0);
    sleep_ms(2);

    // Send a 10 microsecond pulse to TRIG
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    // Measure the time for the echo to return
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 0) {
        start_time = get_absolute_time();
    }

    absolute_time_t end_time = get_absolute_time();
    while (gpio_get(ECHO_PIN) == 1) {
        end_time = get_absolute_time();
    }

    // Calculate the pulse duration in microseconds
    int64_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Calculate the distance in centimeters
    float distance = (pulse_duration / 2.0f) * 0.0343f;

    return distance;
}

// Function to check distance and trigger the buzzer
void check_and_trigger_buzzer(float distance) {
    if (distance > 0 && distance <= DISTANCE_THRESHOLD) {
        gpio_put(BUZZER_PIN, 1); // Turn on the buzzer
    } else {
        gpio_put(BUZZER_PIN, 0); // Turn off the buzzer
    }
}
