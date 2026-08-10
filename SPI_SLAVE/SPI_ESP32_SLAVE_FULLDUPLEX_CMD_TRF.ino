#include <Arduino.h>
#include "driver/spi_slave.h"
#include <string.h>

// ESP32 VSPI pins
#define SPI_MOSI 23
#define SPI_MISO 19
#define SPI_SCLK 18
#define SPI_CS   5

// STM32 commands
#define CMD_LED_CTRL        0x50
#define CMD_LED_SENSORREAD  0x51
#define CMD_LED_READ        0x52
#define CMD_PRINT           0x53
#define CMD_ID_READ          0x54

#define ACK_BYTE 0xF5

// LED
#define LED_PIN 2

void setup()
{
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // SPI bus configuration
    spi_bus_config_t buscfg = {
        .mosi_io_num = SPI_MOSI,
        .miso_io_num = SPI_MISO,
        .sclk_io_num = SPI_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 8
    };

    // SPI slave configuration
    spi_slave_interface_config_t slvcfg = {
        .spics_io_num = SPI_CS,
        .flags = 0,
        .queue_size = 1,
        .mode = 0,       // CPOL = 0, CPHA = 0
        .post_setup_cb = NULL,
        .post_trans_cb = NULL
    };

    esp_err_t ret;

    ret = spi_slave_initialize(
        VSPI_HOST,
        &buscfg,
        &slvcfg,
        SPI_DMA_DISABLED
    );

    if (ret != ESP_OK)
    {
        Serial.printf("SPI slave initialization failed: %d\n", ret);
        while (1);
    }

    Serial.println("================================");
    Serial.println("ESP32 SPI SLAVE READY");
    Serial.println("Waiting for STM32...");
    Serial.println("================================");
}
int state = 1;
void loop()
{
    /*
     * Real wire bytes clocked by the STM32 master for this exchange
     * (SPI_RecieveData() does NOT generate clocks on its own -- it only
     * reads back the result of the most recent SPI_SendData() call --
     * so only every SPI_SendData() call produces an actual byte on SCK):
     *
     * Wire byte 0 -> CMD_CODE        (0x50)          -> ESP32 replies 0x00
     * Wire byte 1 -> dummy           (0xFF)          -> ESP32 replies ACK_BYTE (0xF5)
     * Wire byte 2 -> testData[0]     (0x50)          -> ESP32 replies 0x00
     * Wire byte 3 -> testData[1]     (0xFF)          -> ESP32 replies 0x00
     * Wire byte 4 -> testData[2]     (0x02)          -> ESP32 replies 0x00
     * Wire byte 5 -> testData[3]     (0xAA)          -> ESP32 replies 0x00
     * Wire byte 6 -> testData[4]     (0x55)          -> ESP32 replies 0x00
     *
     * That's 7 bytes total, not 6 -- the previous version reserved a
     * slot for the "dummy read" as if it were its own wire byte, but it
     * isn't, which is why ACK_BYTE was landing one slot too late and
     * ackbyte always read back as 0x00 on the STM32 side.
     */

    const int NUM_BYTES = 4;

    uint8_t rxData[NUM_BYTES] = {0};

    uint8_t txData[NUM_BYTES] = {
        0x00,       // byte 0: response to CMD_LED_CTRL
        ACK_BYTE,   // byte 1: response to the dummy 0xFF -- STM32 reads this as ackbyte
        0x00,       // byte 2
        0x00,       // byte 3
       // 0x00,       // byte 4
       // 0x00,       // byte 5
       // 0x00        // byte 6
    };

    spi_slave_transaction_t transaction;

    memset(&transaction, 0, sizeof(transaction));

    transaction.length = NUM_BYTES * 8;

    transaction.tx_buffer = txData;
    transaction.rx_buffer = rxData;

    esp_err_t ret = spi_slave_transmit(
        VSPI_HOST,
        &transaction,
        portMAX_DELAY
    );

    if (ret != ESP_OK)
    {
        Serial.printf("SPI transaction failed: %d\n", ret);
        return;
    }

    // ----------------------------------------
    // Print received data
    // ----------------------------------------

    Serial.println();
    Serial.println("SPI transaction received:");

    for (int i = 0; i < NUM_BYTES; i++)
    {
        Serial.printf("RX[%d] = 0x%02X\n", i, rxData[i]);
    }

    // ----------------------------------------
    // Check command
    // ----------------------------------------

    if (rxData[0] == CMD_LED_CTRL)
    {
        Serial.println("CMD_LED_CTRL received!");

        // With the current STM32 test code, bytes 2-6 are just
        // placeholder testData (0x50, 0xFF, 0x02, 0xAA, 0x55), so this
        // check won't match LED_PIN/SET yet -- that's expected.
        //
        // Once you send real args[] from the STM32 side again
        // (args[0] = LED_PIN, args[1] = SET), they'll land at:
        //   rxData[2] = LED_PIN
        //   rxData[3] = SET
        // (shifted by one slot vs. the original code, since the dummy
        // exchange now correctly occupies wire byte 1 instead of byte 2)

        uint8_t ledPin  = rxData[2];
        uint8_t command = rxData[3];

        Serial.printf("LED PIN = %d\n", ledPin);
        Serial.printf("COMMAND = 0x%02X\n", command);

        if (ledPin == LED_PIN)
{
    Serial.println("Correct LED pin received");

    state = !state;

    digitalWrite(LED_PIN, state);

    Serial.print("ESP32 LED ");
    Serial.println(state ? "ON" : "OFF");
}
    }
    else
    {
        Serial.println("Unknown command");
    }
}
