#include "gps.h"
#include "hardware/uart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


void setup_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, true);
}

float convert_to_decimal(const char *value) {
    float degrees = 0, minutes = 0;
    sscanf(value, "%2f%f", &degrees, &minutes);
    return degrees + (minutes / 60.0);
}

void convert_utc_to_local(const char *utc_time, char *local_time, int offset_hours) {
    int hours, minutes, seconds;
    sscanf(utc_time, "%2d%2d%2d", &hours, &minutes, &seconds);

    hours += offset_hours;
    if (hours >= 24) {
        hours -= 24; // Wrap around to next day
    } else if (hours < 0) {
        hours += 24; // Wrap around to previous day
    }

    snprintf(local_time, 9, "%02d:%02d:%02d", hours, minutes, seconds);
}

void parse_gprmc(const char *nmea_sentence) {
    char sentence_copy[512];
    strncpy(sentence_copy, nmea_sentence, sizeof(sentence_copy));
    sentence_copy[sizeof(sentence_copy) - 1] = '\0';

    char *token = strtok(sentence_copy, "$GPRMC"); // Split by $GPRMC to separate sentences
    int sentence_index = 0;

    while (token != NULL) {
        char full_sentence[128];
        snprintf(full_sentence, sizeof(full_sentence), "$GPRMC%s", token);

        if (strncmp(full_sentence, "$GPRMC", 6) == 0) {
            char time[11] = {0};
            char latitude[16] = {0};
            char longitude[16] = {0};
            char lat_dir = '\0';
            char lon_dir = '\0';

            sscanf(full_sentence, "$GPRMC,%10[^,],A,%15[^,],%c,%15[^,],%c",
                   time, latitude, &lat_dir, longitude, &lon_dir);

            if (strlen(latitude) > 0 && strlen(longitude) > 0) {
                float latitude_decimal = convert_to_decimal(latitude);
                float longitude_decimal = convert_to_decimal(longitude);

                if (longitude_decimal == 0.0) {
                 return;
                }

                // Convert UTC time to local time (+2 hours)
                char local_time[9] = {0};
                convert_utc_to_local(time, local_time, 2);

                snprintf(latest_gps_data, sizeof(latest_gps_data),
                         "Local Time: %s, Latitude: %.6f %c, Longitude: %.6f %c",
                         local_time, latitude_decimal, lat_dir, longitude_decimal, lon_dir);
            }
        }

        token = strtok(NULL, "$GPRMC"); // Move to next $GPRMC sentence
        sentence_index++;
    }
}


const char* get_gps_data() {
    while (uart_is_readable(UART_ID)) {
        static char buffer[256];
        static int index = 0;

        char c = uart_getc(UART_ID);
        if (c == '\n') {
            buffer[index] = '\0';
             printf("GPS All : %s\n", buffer);
            parse_gprmc(buffer);
            index = 0;
        } else if (index < sizeof(buffer) - 1) {
            buffer[index++] = c;
        }
    }

    return latest_gps_data;
}
