#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define LDR_CHANNEL ADC1_CHANNEL_6   // GPIO34
#define LED_PIN GPIO_NUM_25          // GPIO25
#define THRESHOLD 2000               // ค่า ADC สำหรับตัดสินใจ (0-4095)

static const char *TAG = "LDR_LED";

void app_main(void)
{
    // กำหนด ADC
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(LDR_CHANNEL, ADC_ATTEN_DB_11);

    // กำหนด GPIO สำหรับ LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        int ldr_value = adc1_get_raw(LDR_CHANNEL);
        ESP_LOGI(TAG, "LDR Value: %d", ldr_value);

        if (ldr_value > THRESHOLD) {
            gpio_set_level(LED_PIN, 1);  // เปิด LED
            ESP_LOGI(TAG, "LED ON (Dark)");
        } else {
            gpio_set_level(LED_PIN, 0);  // ปิด LED
            ESP_LOGI(TAG, "LED OFF (Bright)");
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // หน่วง 1 วินาที
    }
}