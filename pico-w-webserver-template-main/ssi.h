  #include "lwip/apps/httpd.h"
  #include "pico/cyw43_arch.h"
  #include "hardware/adc.h"
  #include "gps.h"


  // SSI tags - tag length limited to 8 bytes by default
  const char * ssi_tags[] = {"volt", "temp", "led", "gps"};

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

                printed = snprintf(pcInsert, iInsertLen, "%s", gps_buffer);
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
