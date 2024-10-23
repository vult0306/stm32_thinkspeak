/* ESP HTTP Client Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "protocol_examples_utils.h"
#include "esp_tls.h"
#if CONFIG_MBEDTLS_CERTIFICATE_BUNDLE
#include "esp_crt_bundle.h"
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "freertos/semphr.h"

#include "esp_http_client.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define MAX_HTTP_RECV_BUFFER    512
#define MAX_HTTP_OUTPUT_BUFFER  2048

#define RXD_PIN                 (GPIO_NUM_5)
#define MAX_DATA_BUF_CNT        10

static const char *TAG = "HTTP_CLIENT";
char api_key[] = CONFIG_THINKSPEAK_API_KEY;

typedef struct {
    float Tempt;
    float Humid;
} Dht22_t;

// Create a binary semaphore handle
SemaphoreHandle_t binary_semaphore;
Dht22_t data_buffer[MAX_DATA_BUF_CNT];

void init_uart(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(UART_NUM_1, MAX_DATA_BUF_CNT * sizeof(Dht22_t) * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, UART_PIN_NO_CHANGE, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

static void http_task(void *pvParameters)
{
	char thingspeak_url[] = "https://api.thingspeak.com";
	char data[] = "/update?api_key=%s&field1=%.2f&field2=%.2f";
	char post_data[200];
	esp_err_t err;

	esp_http_client_config_t config = {
		.url = thingspeak_url,
		.method = HTTP_METHOD_GET,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .crt_bundle_attach = esp_crt_bundle_attach,
	};

	esp_http_client_handle_t client = esp_http_client_init(&config);
	esp_http_client_set_header(client, "Content-Type", "application/x-www-form-urlencoded");
	while (1)
	{
        if (xSemaphoreTake(binary_semaphore, portMAX_DELAY)) {
            for (int i=0; i<MAX_DATA_BUF_CNT; i++) {
                strcpy(post_data, "");
                snprintf(post_data, sizeof(post_data), data, api_key, data_buffer[i].Tempt, data_buffer[i].Humid);
                ESP_LOGI(TAG, "post = %s", post_data);
                esp_http_client_set_url(client, post_data);

                err = esp_http_client_perform(client);
                if (err == ESP_OK) {
                    int status_code = esp_http_client_get_status_code(client);
                    if (status_code == 200) {
                        ESP_LOGI(TAG, "Message sent Successfully");
                    } else {
                        ESP_LOGI(TAG, "Message sent Failed");				
                        break;
                    }
                } else {
                    ESP_LOGI(TAG, "Message sent Failed");
                    break;
                }
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
	}

	esp_http_client_cleanup(client);
	vTaskDelete(NULL);

}

static void uart_task(void *arg)
{
    static const char *TAG = "uart_task";
    char print_buf[64];
    esp_log_level_set(TAG, ESP_LOG_INFO);

    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, (uint8_t*)data_buffer, MAX_DATA_BUF_CNT * sizeof(Dht22_t), 1000 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            for (int i=0; i<MAX_DATA_BUF_CNT; i++) {
                sprintf(print_buf, "tempt %.2f, Humid: %.2f", data_buffer[i].Tempt, data_buffer[i].Humid);
                ESP_LOGI(TAG, "%s", print_buf);
            }
            xSemaphoreGive(binary_semaphore);
        }
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // init uart
    init_uart();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
    ESP_LOGI(TAG, "Connected to AP, begin http example");

    // Initialize the binary semaphore
    binary_semaphore = xSemaphoreCreateBinary();

    // Create tasks
    xTaskCreate(&http_task, "http_task",    8192, NULL, 5, NULL);
    xTaskCreate(&uart_task, "uart_task",    4096, NULL, 5, NULL);
}
