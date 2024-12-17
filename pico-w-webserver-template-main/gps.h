
// gps.h
#ifndef GPS_H
#define GPS_H

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include <stdio.h>
#include <string.h>

#define UART_ID uart0            // Using UART0
#define BAUD_RATE 9600           // Neo-6M default baud rate

// Define the pins for UART communication
#define UART_TX_PIN 0            // GP0 as TX (Pico to Neo-6M RX)
#define UART_RX_PIN 1            // GP1 as RX (Neo-6M TX to Pico)
static char latest_gps_data[256] = "Waiting for GPS data...";


void setup_uart();
void parse_gprmc(const char *nmea_sentence);
const char* get_gps_data();

#endif // GPS_H
