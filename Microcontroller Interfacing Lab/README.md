# Microcontroller Interfacing Lab

Bare-metal STM32F446RE examples for communicating between two NUCLEO boards. The projects use direct register access and demonstrate three common serial interfaces.

## Folder structure

```text
Microcontroller Interfacing Lab/
├── I2C/
│   ├── I2C_Switch_Master.c
│   └── I2C_Switch_Slave.c
├── SPI/
│   ├── SPI_Switch_Master.c
│   └── SPI_Switch_Slave.c
├── UART/
│   ├── UART_Switch_Master.c
│   └── UART_Switch_Slave.c
├── I2C_Code_Explain.md
├── Important_Bit_Position.md
└── Resources and Manual/
```

## Examples

### I2C

The master and slave communicate over I2C1 using PB8 (SCL) and PB9 (SDA). The slave address is `0x28`. The master sends commands and polls the slave button state; the boards use PA5 LEDs and PC13 user buttons to show the result.

See [I2C_Code_Explain.md](./I2C_Code_Explain.md) for the protocol, register configuration, bit positions, transaction sequences, wiring, and troubleshooting.

### SPI

The SPI2 examples use PB13 (SCK), PB14 (MISO), and PB15 (MOSI). SPI is master-driven and full-duplex, so the master polls and exchanges a byte on every transfer.

### UART

The USART1 examples use PA9 (TX) and PA10 (RX) at 9600 baud. The same communication behavior is implemented on both boards: a button press sends a command and the other board replies with an acknowledgment.

## Common hardware

- Board: STM32F446RE NUCLEO
- LED: PA5
- User button: PC13, active-low
- Connect GND between both boards.
- Use the interface-specific wiring described in each source file.

## Source and register references

- [I2C code explanation](./I2C_Code_Explain.md)
- [Important bit positions](./Important_Bit_Position.md)
- [I2C master source](./I2C/I2C_Switch_Master.c)
- [I2C slave source](./I2C/I2C_Switch_Slave.c)
- [SPI master source](./SPI/SPI_Switch_Master.c)
- [SPI slave source](./SPI/SPI_Switch_Slave.c)
- [UART master source](./UART/UART_Switch_Master.c)
- [UART slave source](./UART/UART_Switch_Slave.c)
- [STM32F446RE bare-metal register guide](./Resources%20and%20Manual/STM32F446RE%20Bare-Metal%20Register%20Guide.pdf)
- [STM32F446 reference manual](./Resources%20and%20Manual/rm0390-stm32f446xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

## Notes

These are educational bare-metal examples. Timing loops depend on the assumed 16 MHz clock and are not a substitute for production-quality timing or error handling.
