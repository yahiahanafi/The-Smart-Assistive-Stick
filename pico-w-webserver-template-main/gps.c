#include "gps.h"
#include "hardware/uart.h"
#include <string.h>
#include <stdio.h>

void setup_uart() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, true);
}

void parse_gprmc(const char *nmea_sentence) {
    if (strncmp(nmea_sentence, "$GPRMC", 6) != 0) {
        return; // Not a GPRMC sentence
    }

    char sentence_copy[128];
    strncpy(sentence_copy, nmea_sentence, sizeof(sentence_copy));
    sentence_copy[sizeof(sentence_copy) - 1] = '\0';

    char *token = strtok(sentence_copy, ",");
    int field_index = 0;

    char time[11] = {0};
    char latitude[16] = {0};
    char longitude[16] = {0};
    char lat_dir = '\0';
    char lon_dir = '\0';

    while (token != NULL) {
        switch (field_index) {
            case 1: strncpy(time, token, sizeof(time) - 1); break;
            case 3: strncpy(latitude, token, sizeof(latitude) - 1); break;
            case 4: lat_dir = token[0]; break;
            case 5: strncpy(longitude, token, sizeof(longitude) - 1); break;
            case 6: lon_dir = token[0]; break;
        }
        token = strtok(NULL, ",");
        field_index++;
    }

    if (strlen(latitude) > 0 && strlen(longitude) > 0) {
        snprintf(latest_gps_data, sizeof(latest_gps_data),
                 "Time: %s UTC, Latitude: %s %c, Longitude: %s %c",
                 time, latitude, lat_dir, longitude, lon_dir);
    }
}

const char* get_gps_data() {
    while (uart_is_readable(UART_ID)) {
        static char buffer[256];
        static int index = 0;

        char c = uart_getc(UART_ID);
        if (c == '\n') {
            buffer[index] = '\0';
            parse_gprmc(buffer);
            index = 0;
        } else if (index < sizeof(buffer) - 1) {
            buffer[index++] = c;
        }
    }

    return latest_gps_data;
}