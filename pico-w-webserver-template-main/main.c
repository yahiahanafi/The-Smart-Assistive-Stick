#include "lwip/apps/httpd.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwipopts.h"
#include "ssi.h"
#include "cgi.h"
#include "ultrasonic_buzzer.h"
#include "infrared_buzzer.h"
#include "gps.h" // Include GPS header
#include "lwip/netif.h"

// WIFI Credentials - take care if pushing to github!
const char WIFI_SSID[] = "WE_IT.Family";
const char WIFI_PASSWORD[] = "I.S.T.A.Y.0";

int main() {
    stdio_init_all();

    cyw43_arch_init();

    cyw43_arch_enable_sta_mode();

    ultrasonic_buzzer_init();
    infrared_buzzer_init();

    // Initialize GPS system
    setup_uart();

    printf("Ultrasonic, IR Sensor, and GPS Test\n");

    // Connect to the WiFI network - loop until connected
    while(cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000) != 0){
        printf("Attempting to connect...\n");
    }
    // Print a success message once connected
    printf("Connected! \n");
    
    struct netif *netif = netif_list;
    if (netif && !ip4_addr_isany_val(*netif_ip4_addr(netif))) {
        printf("Web server running at http://%s\n", ip4addr_ntoa(netif_ip4_addr(netif)));
    } else {
        printf("Failed to get IP address.\n");
    }
    // Initialise web server
    httpd_init();
    printf("Http server initialised\n");

    // Configure SSI and CGI handler
    ssi_init(); 
    printf("SSI Handler initialised\n");
    cgi_init();
    printf("CGI Handler initialised\n");
    
    // Infinite loop
    while (1) {
        // Ultrasonic functionality
        float distance = measure_distance();
        printf("Distance: %.2f cm\n", distance);
        check_and_trigger_buzzer(distance);

        // IR functionality
        check_and_trigger_ir_buzzer();

        const char *gps_data = get_gps_data(); // Capture the returned GPS data
        if (gps_data != NULL && strlen(gps_data) > 0) {
        printf("GPS Data: %s\n", gps_data); // Print GPS data to console
        }

        sleep_ms(500); // Adjust polling interval as needed
    }
}