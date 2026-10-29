# STM32F446RE I2C Master–Slave (GPIO Pins, Registers, and Bit Positions)

## 1. Important pin numbers and functions

| Pin / Bit          | Function                                       |
| ------------------ | ---------------------------------------------- |
| PA5                | LED output                                     |
| PC13               | Push button input                              |
| PB8                | I2C1 SCL — clock                               |
| PB9                | I2C1 SDA — data                                |
| AF4                | Alternate Function 4; connects PB8/PB9 to I2C1 |
| I2C address `0x28` | Slave address                                  |

## 2. I2C register bit numbers to memorize

These are bit positions inside registers, not physical pins.

| Register   | Bit number | Name    | Function                                          |
| ---------- | ---------- | ------- | ------------------------------------------------- |
| `I2C1_CR1` | 0          | `PE`    | Enable I2C peripheral                             |
| `I2C1_CR1` | 8          | `START` | Generate START condition                          |
| `I2C1_CR1` | 9          | `STOP`  | Generate STOP condition                           |
| `I2C1_CR1` | 10         | `ACK`   | Enable acknowledgment                             |
| `I2C1_CR1` | 15         | `SWRST` | Software reset                                    |
| `I2C1_SR1` | 0          | `SB`    | START condition generated                         |
| `I2C1_SR1` | 1          | `ADDR`  | Address matched / address phase complete          |
| `I2C1_SR1` | 2          | `BTF`   | Byte transfer finished                            |
| `I2C1_SR1` | 4          | `STOPF` | STOP condition detected in slave mode             |
| `I2C1_SR1` | 6          | `RXNE`  | Receive data register not empty                   |
| `I2C1_SR1` | 7          | `TXE`   | Transmit data register empty                      |
| `I2C1_SR1` | 10         | `AF`    | Acknowledge failure                               |
| `I2C1_SR2` | 1          | `BUSY`  | I2C bus is busy                                   |
| `I2C1_SR2` | 2          | `TRA`   | Transfer direction: 0 = receiver, 1 = transmitter |

## 3. GPIO register bits

| Register       | Bit number(s) | Meaning                    |
| -------------- | ------------- | -------------------------- |
| `GPIOA_MODER`  | 11:10         | PA5 mode                   |
| `GPIOB_MODER`  | 17:16         | PB8 mode                   |
| `GPIOB_MODER`  | 19:18         | PB9 mode                   |
| `GPIOB_OTYPER` | 8, 9          | PB8/PB9 output type        |
| `GPIOB_PUPDR`  | 17:16, 19:18  | PB8/PB9 pull-up/down       |
| `GPIOB_AFRH`   | 3:0, 7:4      | PB8/PB9 alternate function |
| `GPIOC_PUPDR`  | 27:26         | PC13 pull-up/down          |
| `GPIOC_IDR`    | 13            | Read PC13 button state     |
| `GPIOA_ODR`    | 5             | Control PA5 LED            |

## 4. I2C communication commands

| Character | Meaning                                  |
| --------- | ---------------------------------------- |
| `'S'`     | Master tells slave to blink 5 times      |
| `'B'`     | Slave reports its button is pressed      |
| `'K'`     | Master acknowledges slave's button press |
| `0x00`    | Slave reports its button is released     |

## 5. I2C timing registers

| Register     | Value | Purpose                                                |
| ------------ | ----- | ------------------------------------------------------ |
| `I2C1_CR2`   | `16`  | Assumed peripheral clock: 16 MHz                       |
| `I2C1_CCR`   | `80`  | Approximately 100 kHz SCL                              |
| `I2C1_TRISE` | `17`  | Maximum rise-time setting for 16 MHz standard-mode I2C |

These timing values assume `PCLK1 = 16 MHz`.

Remember the key sequence: START (bit 8) → address → data → STOP (bit 9). `ADDR`, `RXNE`, `TXE`, and `BTF` are status flags; they are not START/STOP control bits.


