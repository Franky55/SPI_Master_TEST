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


#include "pilote_SPI.h"

// Pins in use
#define GPIO_MOSI 11
#define GPIO_MISO 13
#define GPIO_SCLK 12
#define GPIO_CS 10

void pilote_SPI_initialise()
{
    //spi_device_handle_t handle;

    // Configuration for the SPI bus
    buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1};

    // Configuration for the SPI device on the other side of the bus
    devcfg = {
        .command_bits = 0,
        .address_bits = 0,
        .dummy_bits = 0,
        .clock_speed_hz = 10000000,//20 MHz
        .duty_cycle_pos = 128, // 50% duty cycle
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .input_delay_ns = 1,
        .cs_ena_pretrans = 2,
        .cs_ena_posttrans = 3, // Keep the CS low 3 cycles after transaction
        .queue_size = 1};


    spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    spi_bus_add_device(SPI2_HOST, &devcfg, &handle);

    t.rx_buffer = recvbuf;
    t.tx_buffer = sendbuf;

}



void pilote_SPI_transaction(unsigned char *transmitBuff, unsigned char *receiveBuff, int length)
{
    t.length = length;
    t.rx_buffer = receiveBuff;
    t.tx_buffer = transmitBuff;
    spi_device_transmit(handle, &t);
}