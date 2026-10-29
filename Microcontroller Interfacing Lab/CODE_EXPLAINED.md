# I2C Lab: Code Explained Part by Part

This file walks through `i2c_master.c` and `i2c_slave.c` one section at a time. For each part it says what the code is responsible for and how it works.

---

## 0. The Big Picture

Two STM32F446RE Nucleo boards are wired together over I2C:

```
   MASTER board                         SLAVE board
 ┌──────────────┐    PB8 ── SCL ──    ┌──────────────┐
 │ LED  (PA5)   │    PB9 ── SDA ──    │ LED  (PA5)   │
 │ BTN  (PC13)  │    GND ── GND ──    │ BTN  (PC13)  │
 └──────────────┘   (4.7k pull-ups    └──────────────┘
                     to 3.3V on SCL/SDA)
```

| Event | What happens |
|---|---|
| Boot | Both LEDs ON |
| Master button pressed | Master LED OFF. Master **WRITES** `0x01` to the slave. Slave blinks 5 times, then stays OFF |
| Master button released | Master LED ON. Master **WRITES** `0x00`. Slave LED ON |
| Slave button pressed | Slave LED OFF. Master **READS** `0x01` (it polls every ~20 ms). Master blinks 5 times, then stays OFF |
| Slave button released | Slave LED ON. Master **READS** `0x00`. Master LED ON |

**Key idea:** in I2C only the master can start a transfer. Master → slave data uses a **WRITE**. For slave → master data, the master keeps asking with a **READ**, which is called **polling**.

---

## 1. Register Map (both files)

```c
typedef struct {
    volatile unsigned int MODER;
    volatile unsigned int OTYPER;
    ...
} GPIO_TypeDef;
```

**Responsible for:** letting C code talk to the hardware without any ST header files.

**How it works:**
- Every peripheral (GPIO, RCC, I2C, SysTick) is a block of 32-bit registers at a fixed memory address.
- Each `struct` lists those registers **in the same order and spacing** as the reference manual. The `RESERVED` fields fill gaps so the offsets stay correct.
- `volatile` tells the compiler that the value can change outside the program (the hardware changes it), so the compiler must really read or write memory every time and never cache the value.

```c
#define I2C1_BASE  0x40005400U
#define I2C1       ((I2C_TypeDef *)I2C1_BASE)
```

- The macro casts the base address to a struct pointer. After that, `I2C1->CR1` means "the register at `0x40005400 + offset of CR1`".

| Peripheral | Base address | Used for |
|---|---|---|
| RCC | `0x40023800` | Turning clocks on |
| GPIOA | `0x40020000` | LED (PA5) |
| GPIOB | `0x40020400` | I2C pins (PB8, PB9) |
| GPIOC | `0x40020800` | Button (PC13) |
| I2C1 | `0x40005400` | The I2C peripheral |
| SysTick | `0xE000E010` | Delay timer (built into the ARM core) |

---

## 2. Bit Definitions and Settings (both files)

```c
#define I2C_CR1_START   (1U << 8)
#define I2C_SR1_SB      (1U << 0)
...
#define SLAVE_ADDR      0x30U
#define I2C_TIMEOUT     100000U
#define CMD_RELEASED    0x00U
#define CMD_PRESSED     0x01U
```

**Responsible for:** giving names to bit positions and to settings you might want to change.

**How it works:**
- `(1U << n)` makes a mask with only bit *n* set. `reg |= mask` sets the bit, `reg &= ~mask` clears it, and `reg & mask` tests it.
- `SLAVE_ADDR` must be **the same in both files**, or the master gets a NACK.
- `I2C_TIMEOUT` is the maximum number of loop passes before a wait gives up, so the program can never hang.
- `CMD_PRESSED` / `CMD_RELEASED` are the **protocol**: the one-byte "language" both boards agree on. The same encoding is used in both directions.

---

## 3. `GPIO_Init()` (identical in both files)

**Responsible for:** setting up the LED, button, and I2C pins.

### 3a. Turn on the GPIO clocks
```c
RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN;
```
A peripheral does nothing until its clock is on. This turns on ports A, B, and C.

### 3b. PA5 = LED output
```c
GPIOA->MODER &= ~(3U << (5 * 2));   // clear the 2 mode bits of pin 5
GPIOA->MODER |=  (1U << (5 * 2));   // 01 = general-purpose output
GPIOA->OTYPER &= ~(1U << 5);        // push-pull
```
Each pin has **2 bits** in `MODER`, so pin *n* sits at bit position `n*2`. The pattern is "clear first, then set".

### 3c. PC13 = button input
```c
GPIOC->MODER &= ~(3U << (13 * 2));  // 00 = input
GPIOC->PUPDR &= ~(3U << (13 * 2));  // no internal pull (the board has its own)
```

### 3d. PB8 / PB9 = I2C pins
```c
GPIOB->MODER  |= (2U << 16) | (2U << 18);   // 10 = alternate function
GPIOB->OTYPER |= (1U << 8) | (1U << 9);     // OPEN-DRAIN (required for I2C)
GPIOB->PUPDR  |= (1U << 16) | (1U << 18);   // internal pull-up
GPIOB->AFR[1] |= (4U << 0) | (4U << 4);     // AF4 = I2C1
```
- **Alternate function mode** hands control of the pin to the I2C peripheral instead of normal GPIO.
- **Open-drain** means a pin can only pull the line LOW. A resistor pulls it HIGH. This is what I2C needs.
- **AFR[1]** covers pins 8–15 with 4 bits per pin. Writing value 4 selects I2C1 on PB8 and PB9.
- The internal pull-ups are weak (~40 kΩ), so **external 4.7 kΩ pull-ups** are still needed.

---

## 4. I2C Initialisation

### Master: `I2C1_Init()`
```c
RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;   // 1. clock on
I2C1->CR1 |= I2C_CR1_SWRST;           // 2. reset the peripheral
I2C1->CR1 &= ~I2C_CR1_SWRST;
I2C1->CR2   = 16;                     // 3. APB1 clock = 16 MHz
I2C1->CCR   = 80;                     // 4. 100 kHz SCL
I2C1->TRISE = 17;                     // 5. max rise time
I2C1->CR1 |= I2C_CR1_PE;              // 6. enable
I2C1->CR1 |= I2C_CR1_ACK;             // 7. enable ACK
```

| Step | Why |
|---|---|
| Software reset | Clears any stuck state left from earlier runs |
| `CR2 = 16` | The peripheral must know its input clock (16 MHz HSI) to time the bus |
| `CCR = 80` | SCL high time = low time = 80 × 62.5 ns = 5 µs, so one period is 10 µs = **100 kHz** |
| `TRISE = 17` | Standard mode allows a 1000 ns rise time: 1000 ns / 62.5 ns + 1 = 17 |
| PE before ACK | The ACK bit is cleared by hardware when PE = 0, so ACK must be set **after** enabling |

### Slave: `I2C1_Slave_Init()`
It is the same as the master's, plus:
```c
I2C1->OAR1  = 0;
I2C1->OAR1 |= (1U << 14);          // reference manual says keep bit 14 = 1
I2C1->OAR1 |= (SLAVE_ADDR << 1);   // "my address is 0x30"
```
`OAR1` = **Own Address Register**. The hardware compares every address on the bus with this value and responds automatically (sets `ADDR` and ACKs) when it matches. The address goes in bits 7:1, which is why it is shifted left by 1.

### Master: `I2C1_Recover()`
```c
I2C1->CR1 |= I2C_CR1_STOP;  delay_ms(2);  I2C1_Init();
```
This is called when a START times out (error 1). It sends a STOP to free the bus, then re-initialises everything.

---

## 5. `I2C1_MasterWrite(cmd)`: Master → Slave

**Responsible for:** sending one command byte (`0x00` or `0x01`) to the slave.

```
Bus:   S | 0x60 (0x30<<1 | W) | ACK | cmd | ACK | P
```

| Step | Code | What happens on the bus / in hardware |
|---|---|---|
| 1 | `CR1 |= START` and wait for `SB` | Hardware creates a START condition. `SB = 1` means it is done |
| 2 | `DR = (0x30 << 1) | 0` | Sends the address byte `0x60` (R/W = 0, write) |
| 3 | Wait for `ADDR` **or** `AF` | `ADDR` = the slave ACKed. `AF` = nobody answered (NACK), so return error 2 |
| 4 | Read `SR1`, then `SR2` | This exact read sequence **clears ADDR** and lets the transfer continue |
| 5 | Wait for `TXE`, then `DR = cmd` | `TXE` = the data register is empty, so the byte can be loaded |
| 6 | Wait for `BTF` | Byte transfer finished: the byte was sent and ACKed |
| 7 | `CR1 |= STOP` | Hardware creates a STOP condition and the bus is free again |

Every `while` is protected like this:
```c
timeout = I2C_TIMEOUT;
while (!(I2C1->SR1 & I2C_SR1_SB)) {
    if (--timeout == 0) { i2c_last_error = 1; return 0; }
}
```
If the flag never comes, the function records **why** in `i2c_last_error` and returns 0 instead of hanging.

---

## 6. `I2C1_MasterRead(&out)`: Slave → Master

**Responsible for:** reading the slave's button state (one byte).

```
Bus:   S | 0x61 (0x30<<1 | R) | ACK | data (from slave) | NACK | P
```

| Step | Code | Why |
|---|---|---|
| 1 | START and wait for `SB` | Same as the write |
| 2 | `DR = (0x30 << 1) | 1` | Address byte `0x61` (R/W = 1, read) |
| 3 | Wait for `ADDR` / `AF` | Check that the slave answered |
| 4 | `CR1 &= ~ACK` | **Before** the byte arrives, arrange for it to be NACKed, because it is the last (and only) byte |
| 5 | Read `SR1`, `SR2` | Clears `ADDR`, and the slave starts sending |
| 6 | `CR1 |= STOP` | Request the STOP **now**, so it goes out right after the byte |
| 7 | Wait for `RXNE`, then `*out = DR` | The byte has arrived. Read it |
| 8 | `CR1 |= ACK` | Turn ACK back on for future transfers |

**Why this order?** For a single-byte read, the STM32 reference manual says ACK must be cleared and STOP set **before** the byte finishes. If you wait until `RXNE` first, the master has already started clocking a second byte, and the bus gets out of step.

---

## 7. Master `main()` Loop

```c
GPIO_Init();  I2C1_Init();  LED_On();
while (1) { ...part A... ...part B... delay_ms(20); }
```

### Part A: own button → own LED + tell the slave
```c
button_now = Button_Pressed();
if (button_now && !last_own_button)      { LED_Off(); I2C1_MasterWrite(CMD_PRESSED);  }
else if (!button_now && last_own_button) { LED_On();  I2C1_MasterWrite(CMD_RELEASED); }
last_own_button = button_now;
```
- **Edge detection:** comparing `button_now` with `last_own_button` means a message is sent **only when the state changes**, not on every loop pass while the button is held.
- The LED changes **first** (instant, local), and then the I2C message is sent.
- If the write fails with error 1 (bus stuck), `I2C1_Recover()` runs.

### Part B: poll the slave's button
```c
if (I2C1_MasterRead(&remote_state)) {
    if (remote_state == CMD_PRESSED && last_remote_button == 0)
        { BlinkLED(5); LED_Off(); last_remote_button = 1; }
    else if (remote_state == CMD_RELEASED && last_remote_button == 1)
        { LED_On(); last_remote_button = 0; }
}
```
- A read happens **every loop pass (~every 20 ms)**.
- `last_remote_button` gives edge detection again: the master blinks once per press, not 50 times per second.
- A failed read (for example error 6 while the slave is busy blinking) is simply retried on the next pass.

### `delay_ms(20)`
This sets the loop speed (~50 polls per second) and also works as simple **button debounce**: switch bounce settles within the 20 ms gap.

---

## 8. Slave `main()` Loop

```c
GPIO_Init();  I2C1_Slave_Init();  LED_On();
while (1) { ...part A... ...part B... delay_ms(10); }
```

### Part A: own button → own LED + remember the state
```c
if (button_now && !last_own_button)      LED_Off();
else if (!button_now && last_own_button) LED_On();
own_button_state = button_now;   // this is what the master will read
```
The slave cannot send this to the master on its own, so it just **stores** the state in `own_button_state`, ready for the next time the master asks.

### Part B: was I addressed? (non-blocking)
```c
if (I2C1->SR1 & I2C_SR1_ADDR) {
    (void)I2C1->SR1;
    unsigned int sr2 = I2C1->SR2;   // clears ADDR
    if (sr2 & I2C_SR2_TRA)  { ...master is READING... }
    else                    { ...master is WRITING... }
}
```
- This is an **`if`, not a `while`**. If the master has not called, the slave skips this part and keeps watching its button. The slave is never stuck waiting.
- The hardware sets `ADDR` automatically when the address on the bus matches `OAR1`.
- `TRA` (transmitter bit in `SR2`) shows the direction:
  - `TRA = 1`: the master sent R/W = 1, so the slave must **transmit**
  - `TRA = 0`: the master sent R/W = 0, so the slave must **receive**

#### Case 1: master is READING (TRA = 1)
| Step | Code | Why |
|---|---|---|
| 1 | Wait for `TXE` | The data register is ready |
| 2 | `DR = own_button_state` | Hand the button byte to the hardware, which shifts it out on SDA |
| 3 | Wait for `AF` | The master NACKs the last byte, which shows up as `AF`. That is how the slave knows the read is over (STOPF is **not** set in this case) |
| 4 | Clear `AF` | Ready for the next transfer |

#### Case 2: master is WRITING (TRA = 0)
| Step | Code | Why |
|---|---|---|
| 1 | Wait for `RXNE`, then `cmd = DR` | A byte has arrived. Read the command |
| 2 | Wait for `STOPF` | The master sent STOP, so the transfer is complete |
| 3 | Read `SR1`, then write `CR1` | This is the required sequence to **clear STOPF** |
| 4 | Act on `cmd` | `0x01`: `BlinkLED(5)` then LED OFF. `0x00`: LED ON |

If any step times out, the slave records an error and calls `I2C1_Slave_Init()` to **reset itself** so it never stays stuck.

> Note: the header comment lists error 2 for a STOPF timeout, but the code currently sets `1` for both the RXNE and STOPF timeouts.

---

## 9. Helper Functions (both files)

### `Button_Pressed()`
```c
return ((GPIOC->IDR & (1U << 13)) == 0U);
```
Reads the PC13 input. The button is **active LOW**: pressing it connects the pin to GND, so a reading of 0 means pressed.

### `LED_On()` / `LED_Off()`
```c
GPIOA->BSRR = (1U << 5);          // set PA5   -> LED ON
GPIOA->BSRR = (1U << (5 + 16));   // reset PA5 -> LED OFF
```
`BSRR` = Bit Set/Reset Register. The low 16 bits **set** pins and the high 16 bits **reset** them. Each write is a single **atomic** operation, so there is no read-modify-write as with `ODR`.

### `BlinkLED(times)`
A loop of `ON → 500 ms → OFF → 500 ms`, which makes 1 second per blink. It is a fixed number of steps, so it always finishes.

### `delay_ms(ms)`: SysTick timer
```c
SYSTICK->LOAD = 16000 - 1;        // count 16000 ticks = 1 ms at 16 MHz
SYSTICK->VAL  = 0;                // start from 0
SYSTICK->CTRL = ENABLE | CLKSOURCE;
for (i = 0; i < ms; i++)
    while (!(SYSTICK->CTRL & COUNTFLAG)) { }   // wait for each 1 ms rollover
SYSTICK->CTRL = 0;                // stop
```
SysTick is a 24-bit down-counter built into every Cortex-M core. `COUNTFLAG` becomes 1 each time it reaches 0 (every 1 ms) and is cleared when read, so each loop pass waits exactly 1 ms.

---

## 10. Full Flow Example: Master Button Pressed

```
MASTER                                   BUS                         SLAVE
──────                                   ───                         ─────
Button_Pressed() = 1 (edge)
LED_Off()
I2C1_MasterWrite(0x01):
  START ──────────────────────────────►  S
  DR = 0x60 ──────────────────────────►  0x60 ───────────────► address matches OAR1
                                                               hardware ACKs, ADDR = 1
                                         ◄──────── ACK
  ADDR = 1, clear it                                           main loop sees ADDR,
                                                               TRA = 0 → receive
  DR = 0x01 ──────────────────────────►  0x01 ───────────────► RXNE = 1, cmd = 0x01
                                         ◄──────── ACK
  BTF = 1
  STOP ───────────────────────────────►  P ──────────────────► STOPF = 1, clear it
return 1                                                       BlinkLED(5), LED_Off()
```

## 11. Full Flow Example: Slave Button Pressed

```
MASTER                                   BUS                         SLAVE
──────                                   ───                         ─────
                                                               Button_Pressed() = 1
                                                               LED_Off()
                                                               own_button_state = 1
(every 20 ms) I2C1_MasterRead():
  START ──────────────────────────────►  S
  DR = 0x61 ──────────────────────────►  0x61 ───────────────► match, ACK, ADDR = 1
                                         ◄──────── ACK
  clear ACK, clear ADDR, set STOP                              main loop sees ADDR,
                                                               TRA = 1 → transmit
                                         ◄──────── 0x01 ────── DR = own_button_state
  RXNE = 1, out = 0x01
                                         NACK ───────────────► AF = 1, clear it
                                         P
remote_state = 1, last was 0 (edge)
BlinkLED(5), LED_Off()
```

---

## 12. Error Codes

### Master: `i2c_last_error`
| Code | Meaning | Usual cause |
|---|---|---|
| 0 | OK | |
| 1 | START timeout (`SB`) | No pull-ups, bus stuck LOW, wiring or GND fault |
| 2 | Address NACK (`AF`) | Slave not running, or the address does not match |
| 3 | `ADDR` timeout | Slave stretching the clock for too long |
| 4 | `TXE` timeout | Write stalled |
| 5 | `BTF` timeout | Write stalled |
| 6 | `RXNE` timeout | Slave busy blinking. **Normal**, retried in 20 ms |

### Slave: `i2c_slave_last_error`
| Code | Meaning |
|---|---|
| 0 | OK |
| 1 | `RXNE` or `STOPF` timeout during a write from the master |
| 3 | `TXE` or `AF` timeout during a read by the master |

---

## 13. Known Limitation

`BlinkLED(5)` takes **5 seconds** and blocks the loop. While the slave is blinking, it cannot answer the bus, so master reads and writes in that window time out and are skipped. Because polling repeats every 20 ms and buttons are pressed at human speed, this is rarely noticeable. A better design would use a **timer interrupt** for blinking and an **I2C interrupt** for the bus.
