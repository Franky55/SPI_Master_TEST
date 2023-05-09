
#ifndef PILOTESPI_H
#define PILOTESPI_H

spi_device_handle_t handle;

    // Configuration for the SPI bus
    spi_bus_config_t buscfg;

    // Configuration for the SPI device on the other side of the bus
    spi_device_interface_config_t devcfg;

    unsigned char recvbuf[129];

    unsigned char sendbuf[20];

    spi_transaction_t t;

    void pilote_SPI_initialise();
    void pilote_SPI_transaction(unsigned char *transmitBuff, unsigned char *receiveBuff, int length);


#endif