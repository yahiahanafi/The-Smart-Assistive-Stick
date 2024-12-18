  #include "lwip/apps/httpd.h"
  #include "pico/cyw43_arch.h"
  #include "hardware/adc.h"
  #include "gps.h"


  // SSI tags - tag length limited to 8 bytes by default
  const char * ssi_tags[] = {"volt", "temp", "led", "gps", "long"};

double extract_longitude(const char* gps_data) {
    if (!gps_data) return 0.0;

    // Find the substring "Longitude: "
    const char* long_start = strstr(gps_data, "Longitude: ");
    if (long_start) {
        // Move pointer past "Longitude: "
        long_start += strlen("Longitude: ");

        // Extract longitude until the next space or end of string
        char longitude_str[32] = {0};
        size_t i = 0;

        while (*long_start && *long_start != ' ' && i < sizeof(longitude_str) - 1) {
            longitude_str[i++] = *long_start++;
        }

        longitude_str[i] = '\0'; // Null-terminate the string

        // Convert to a double
        return atof(longitude_str);
    }

    return 0.0; // Return 0 if "Longitude: " is not found
}



u16_t ssi_handler(int iIndex, char *pcInsert, int iInsertLen) {
    size_t printed;

    switch (iIndex) {
        case 0: // volt
            {
                const float voltage = adc_read() * 3.3f / (1 << 12);
                printed = snprintf(pcInsert, iInsertLen, "%f", voltage);
            }
            break;
        case 1: // temp
            {
                const float voltage = adc_read() * 3.3f / (1 << 12);
                const float tempC = 27.0f - (voltage - 0.706f) / 0.001721f;
                printed = snprintf(pcInsert, iInsertLen, "%f", tempC);
            }
            break;
        case 2: // led
            {
                bool led_status = cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
                printed = snprintf(pcInsert, iInsertLen, led_status ? "ON" : "OFF");
            }
            break;
        case 3: // gps
            {
                // Retrieve and format GPS data
                static char gps_buffer[256];
                const char *gps_data = get_gps_data(); // Use a helper function
                snprintf(gps_buffer, sizeof(gps_buffer), "%s", gps_data);

                printed = snprintf( pcInsert, iInsertLen, "%s", gps_buffer);
            }
            break;
        case 4: // long
            {
                // Retrieve and format GPS data
                static char gps_buffer[256];
                const char *gps_data = get_gps_data(); // Use a helper function

                // Extract longitude
                double longitude = extract_longitude(gps_buffer);

                // Convert longitude to string and format it
                char longitude_str[32];
                snprintf(longitude_str, sizeof(longitude_str), "%f", longitude);

                // Store it in pcInsert
                printed = snprintf(pcInsert, iInsertLen, "%s", longitude_str);
            }
            break;
        default:
            printed = 0;
            break;
    }

    return (u16_t)printed;
}




  // Initialise the SSI handler
  void ssi_init() {
    // Initialise ADC (internal pin)
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    http_set_ssi_handler(ssi_handler, ssi_tags, LWIP_ARRAYSIZE(ssi_tags));
  }
