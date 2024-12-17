#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include <stdio.h>

#define UART_ID uart0            // Using UART0
#define BAUD_RATE 9600           // Neo-6M default baud rate

// Define the pins for UART communication
#define UART_TX_PIN 0            // GP0 as TX (Pico to Neo-6M RX)
#define UART_RX_PIN 1            // GP1 as RX (Neo-6M TX to Pico)

void setup_uart() {
    // Initialize UART
    uart_init(UART_ID, BAUD_RATE);

    // Set GPIO pins for UART
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Configure UART with default settings
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, true);

    // Enable UART receive interrupt (optional)
    // irq_set_enabled(UART0_IRQ, true);
}

void read_gps_data() {
    char buffer[256]; // Buffer for storing GPS data
    int index = 0;

    while (true) {
        // Check if there's data available in UART
        while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);

            // Check for newline (end of an NMEA sentence)
            if (c == '\n') {
                buffer[index] = '\0'; // Null-terminate the string
                printf("GPS Data: %s\n", buffer);
                index = 0; // Reset index for next sentence
            } else if (index < sizeof(buffer) - 1) {
                buffer[index++] = c; // Store character in buffer
            }
        }
    }
}

int main() {
    stdio_init_all(); // Initialize USB for debug output
    setup_uart();     // Configure UART for GPS

    printf("Waiting for GPS data...\n");

    // Start reading GPS data
    read_gps_data();

    return 0; // This line is never reached
}