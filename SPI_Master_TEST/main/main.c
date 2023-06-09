// Master as transmitter for SPI communication
//https://github.com/SIMS-IOT-Devices/FreeRTOS-ESP-IDF-SPI/blob/main/Master_transmitter_output.c
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/igmp.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "soc/rtc_periph.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "spi_flash_mmap.h"

#include "driver/gpio.h"
#include "esp_intr_alloc.h"

// Pins in use
#define GPIO_MOSI 11
#define GPIO_MISO 13
#define GPIO_SCLK 12
#define GPIO_CS 10

// Main application
void app_main(void)
{
    spi_device_handle_t handle;

    // Configuration for the SPI bus
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    // Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 10000000,//60 MHz
        .duty_cycle_pos = 128, // 50% duty cycle
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .cs_ena_pretrans = 400,
        .input_delay_ns = 100,
        .cs_ena_posttrans = 3, // Keep the CS low 3 cycles after transaction
        .queue_size = 1};




    char recvbuf[129] = "";
    memset(recvbuf, 0, 33);

    char sendbuf[20] = {0};

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI3_HOST, &devcfg, &handle);
int i = 0;
    printf("Master output:\n");
    while (1)
    {
        snprintf(sendbuf, sizeof(sendbuf), "Master bitch");
        t.length = sizeof(sendbuf) * 8;
        t.rx_buffer = recvbuf;
        t.tx_buffer = sendbuf;
        spi_device_transmit(handle, &t);
        printf("Transmitted: %s\n", sendbuf);
        printf("Received: %d\n", recvbuf[0]);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}