# STM32F401 Bare-Metal SPI Master ↔ ESP32 SPI Slave

A bare-metal SPI communication project demonstrating **full-duplex SPI
communication between an STM32F401CCU6 and an ESP32**.

The STM32 operates as the **SPI Master** and sends an LED-control
command to the ESP32 Slave. The ESP32 responds with an ACK byte and
receives the LED pin and control argument in the same SPI transaction.

## Communication Flow

The following diagram shows the complete command/response sequence
implemented in the project:

![STM32 Master to ESP32 Slave SPI
Communication](SPI_Communication_Flow.png)

## Hardware

  Device          Role                         Interface
  --------------- ---------------------------- -------------
  STM32F401CCU6   SPI Master                   SPI2
  ESP32           SPI Slave                    VSPI
  Push Button     Transfer trigger             STM32 PA0
  LED             Output controlled by ESP32   ESP32 GPIO2

### SPI Connections

  STM32F401CCU6     ESP32
  ----------------- --------
  PB13 --- SCK      GPIO18
  PB14 --- MISO     GPIO19
  PB15 --- MOSI     GPIO23
  PB12 --- NSS/CS   GPIO5
  GND               GND

SPI configuration:

-   Master: STM32F401CCU6
-   Slave: ESP32
-   SPI Mode: 0
-   CPOL: 0
-   CPHA: 0
-   Data frame: 8-bit
-   Communication: Full Duplex
-   STM32 SPI peripheral: SPI2
-   ESP32 interface: VSPI

## Command Protocol

The project uses a simple command protocol on top of SPI.

  Byte        Value Meaning
  -------- -------- ------------------------------------------------
  Byte 0     `0x50` `CMD_LED_CTRL` --- LED control command
  Byte 1     `0xFF` Dummy byte used to generate clocks for the ACK
  Byte 2     `0x02` LED pin number
  Byte 3     `0x01` LED control argument (`SET`)

The complete STM32 → ESP32 data sequence is:

``` text
0x50  0xFF  0x02  0x01
 │     │     │     │
 │     │     │     └── LED command argument
 │     │     └──────── LED GPIO number
 │     └────────────── Dummy byte / clock generation
 └──────────────────── LED control command
```

The ESP32 prepares the following response bytes:

``` text
0x00  0xF5  0x00  0x00
 │     │     │     │
 │     │     │     └── Dummy response
 │     │     └──────── Dummy response
 │     └────────────── ACK = 0xF5
 └──────────────────── Response to command byte
```

## Transaction Sequence

### 1. Button Press

The STM32 monitors PA0. When the button is pressed:

``` text
PA0 = LOW
```

A short software delay is used for debounce.

### 2. Enable SPI

The STM32 enables SPI2 and starts the transaction.

### 3. Send Command

The STM32 sends:

``` text
0x50
```

The ESP32 receives it as:

``` text
rxData[0] = 0x50
```

This identifies the operation as `CMD_LED_CTRL`.

### 4. Generate Clocks and Receive ACK

The STM32 sends:

``` text
0xFF
```

The dummy byte is not an application command. Its purpose is to generate
SPI clock pulses so the STM32 can receive the ESP32 response.

The ESP32 returns:

``` text
0xF5
```

The STM32 stores this in `ackbyte` and verifies:

``` c
if (ackbyte == 0xF5)
```

Only after receiving the expected ACK does the STM32 continue with the
LED arguments.

### 5. Send LED Arguments

The STM32 sends:

``` text
args[0] = 0x02;
args[1] = 0x01;
```

The ESP32 receives:

``` text
rxData[2] = 0x02;
rxData[3] = 0x01;
```

The ESP32 then checks the LED pin and toggles the GPIO2 LED state.

### 6. Consume Response Bytes

Because SPI is full duplex, the two argument bytes also produce two
received bytes on the STM32.

The STM32 therefore reads them into:

``` c
uint8_t dummy_rx[2];
SPI_RecieveData(SPI2, dummy_rx, 2);
```

These bytes are not currently used by the application.

### 7. Finish Transaction

The STM32 waits until:

``` text
SPI_BUSY_FLAG = 0
```

Then SPI2 is disabled and the code waits for the button to be released
before accepting another command.

## Full-Duplex Data Transfer

SPI transfers data in both directions at the same time.

For the four-byte transaction:

``` text
             STM32 MASTER                 ESP32 SLAVE

Transfer 0
TX: 0x50  ─────────────────────────────►  RX[0] = 0x50
RX: 0x00  ◄─────────────────────────────  TX[0] = 0x00

Transfer 1
TX: 0xFF  ─────────────────────────────►  RX[1] = 0xFF
RX: 0xF5  ◄─────────────────────────────  TX[1] = 0xF5

Transfer 2
TX: 0x02  ─────────────────────────────►  RX[2] = 0x02
RX: 0x00  ◄─────────────────────────────  TX[2] = 0x00

Transfer 3
TX: 0x01  ─────────────────────────────►  RX[3] = 0x01
RX: 0x00  ◄─────────────────────────────  TX[3] = 0x00
```

SPI itself does not understand that `0x50` means LED control or that
`0x02` means GPIO2. The application protocol assigns meaning to each
byte according to its position in the transaction.

## Example ESP32 Output

A successful transaction appears as:

``` text
SPI transaction received:

RX[0] = 0x50
RX[1] = 0xFF
RX[2] = 0x02
RX[3] = 0x01

CMD_LED_CTRL received!
LED PIN = 2
COMMAND = 0x01
Correct LED pin received
ESP32 LED ON
```

On the next valid command, the current ESP32 implementation toggles the
LED state:

``` text
ESP32 LED OFF
```

## Project Structure

``` text
STM32F401_BareMetal_Drivers/
│
├── drivers/
│   ├── INC/
│   │   ├── STM32F401CCUx.h
│   │   ├── stm32f401ccux_gpio_driver.h
│   │   └── spi.h
│   │
│   └── SRC/
│       ├── stm32f401ccux_gpio_driver.c
│       └── spi.c
│
├── Src/
│   ├── SPI_FullDuplex_Test.c
│   └── spi_send_data_test.c
│
├── SPI_SLAVE/
│   └── SPI_ESP32_SLAVE_FULLDUPLEX_CMD_TRF.ino
│
└── README.md
```

## Key Implementation Concepts

-   Register-level STM32 peripheral configuration
-   Bare-metal SPI driver development
-   SPI full-duplex communication
-   SPI Master/Slave communication
-   Hardware NSS/CS handling
-   SPI TXE, RXNE and BUSY flag handling
-   Command/ACK based communication protocol
-   Dummy-byte clock generation
-   Sequential TX/RX buffer handling
-   Software button debouncing
-   STM32-to-ESP32 hardware debugging

## Current Command Definitions

The protocol reserves the following command codes:

``` c
#define CMD_LED_CTRL        0x50
#define CMD_LED_SENSORREAD  0x51
#define CMD_LED_READ        0x52
#define CMD_PRINT           0x53
#define CMD_ID_READ         0x54
```

Currently, the demonstrated transaction implements:

``` text
0x50 → LED Control
```

The other command codes are reserved for future functionality.

## Learning Outcome

This project demonstrates how a custom application protocol can be built
on top of SPI. It also shows the difference between the SPI hardware
layer, which transfers bytes, and the application layer, which assigns
meaning to those bytes.

The project was developed and tested using actual STM32 and ESP32
hardware.
