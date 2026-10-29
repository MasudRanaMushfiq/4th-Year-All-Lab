# I2C Lab: Simple Explanation (Easy Version)

This file explains, in very simple words, how the code works, what each register does, and how the registers are set, cleared, and checked.

---

## 1. What Is a Register?

Think of a **register as a row of 32 light switches** inside the chip.

```
bit:  31 30 29 ... 10  9  8  7  6  5  4  3  2  1  0
      [0][0][0]...[0][0][0][0][0][0][0][0][0][0][0]
```

- Each switch is a **bit**, which is either **0 (OFF)** or **1 (ON)**.
- Each register has a fixed **address**, like a house number.
- When **you** flip a switch, the hardware does something.
- Some switches are **flipped by the hardware itself** to tell you something, like a notification light.

There are only two kinds of register:

| Kind | Who changes it | Example |
|---|---|---|
| **Control register** | **You** write it to give orders | `CR1`: "make a START!" |
| **Status register** | **Hardware** writes it, you read it | `SR1`: "START done ✓" |

---

## 2. The Only 3 Operations You Need

**SET a bit (turn it ON):**
```c
REG |= (1U << 8);     // turn ON switch 8, leave the others alone
```

**CLEAR / RESET a bit (turn it OFF):**
```c
REG &= ~(1U << 8);    // turn OFF switch 8, leave the others alone
```

**CHECK a bit (is it ON?):**
```c
if (REG & (1U << 8))  // true if switch 8 is ON
```

`(1U << 8)` means "a 1 moved to position 8", in other words "switch number 8".

**The whole program is just these 3 operations, repeated.**

---

## 3. Step 1: Turn On the Power (RCC)

**RCC = Reset and Clock Control**

Every part of the chip is **asleep** at the start to save power. You must wake it up first.

```c
RCC->AHB1ENR |= (1<<0) | (1<<1) | (1<<2);  // wake up GPIOA, GPIOB, GPIOC
RCC->APB1ENR |= (1<<21);                   // wake up I2C1
```

| Register | Full form | Job |
|---|---|---|
| `AHB1ENR` | **AHB1 peripheral clock ENable Register** | Turns on the clock for GPIO ports |
| `APB1ENR` | **APB1 peripheral clock ENable Register** | Turns on the clock for I2C1 |

- **AHB = Advanced High-performance Bus** (the fast internal road)
- **APB = Advanced Peripheral Bus** (the slower internal road)

> **RCC is the main power switchboard.** If you skip this step, nothing else works.

---

## 4. Step 2: Set Up the Pins (GPIO)

**GPIO = General Purpose Input/Output**

Every pin can be used in different ways. You choose with these registers:

| Register | Full form | Question it answers | Bits per pin |
|---|---|---|---|
| `MODER` | **MODE Register** | Input? Output? Or given to a peripheral? | 2 |
| `OTYPER` | **Output TYPE Register** | Normal output (push-pull) or open-drain? | 1 |
| `OSPEEDR` | **Output SPEED Register** | How fast the pin can switch | 2 |
| `PUPDR` | **Pull-Up / Pull-Down Register** | Add a pull-up or pull-down resistor? | 2 |
| `AFR` | **Alternate Function Register** | Which peripheral gets the pin? | 4 |
| `IDR` | **Input Data Register** | **Read** the pin (is it 0 or 1?) | 1 |
| `BSRR` | **Bit Set / Reset Register** | **Write** the pin (set it to 1 or 0) | 1 |

`MODER` values (2 bits per pin):

| Value | Meaning |
|---|---|
| `00` | Input |
| `01` | Output |
| `10` | Alternate Function (the pin is given to a peripheral such as I2C) |
| `11` | Analog |

### LED on PA5 = output
(**PA5 = Port A, pin 5**. **LED = Light Emitting Diode**)
```c
GPIOA->MODER &= ~(3 << 10);  // clear the 2 bits of pin 5 (5 × 2 = bit 10)
GPIOA->MODER |=  (1 << 10);  // 01 = output
```
Why "clear first, then set"? It wipes out any old value, then writes the new one.

### Button on PC13 = input
(**PC13 = Port C, pin 13**)
```c
GPIOC->MODER &= ~(3 << 26);  // 00 = input   (13 × 2 = bit 26)
```

### PB8 and PB9 = I2C pins
(**PB8 = SCL**, **PB9 = SDA**)
```
MODER  → 10      // "give this pin to a peripheral"
OTYPER → 1       // open-drain (I2C rule: a pin can only pull LOW)
PUPDR  → 01      // pull-up
AFR    → 4       // AF4 = Alternate Function 4 = I2C1
```

### Turning the LED on/off and reading the button
```c
GPIOA->BSRR = (1 << 5);           // LED ON  (low half of BSRR = SET pin)
GPIOA->BSRR = (1 << 21);          // LED OFF (high half = RESET pin, 5 + 16 = 21)
if ((GPIOC->IDR & (1<<13)) == 0)  // button pressed (pressed = 0, active LOW)
```

---

## 5. Step 3: Set Up I2C

**I2C = Inter-Integrated Circuit**

### Control registers (YOU write them)

| Register / Bit | Full form | What it does |
|---|---|---|
| `CR1` | **Control Register 1** | Main ON/OFF and command register |
| `CR1.PE` (bit 0) | **Peripheral Enable** | **ON/OFF switch** for I2C |
| `CR1.START` (bit 8) | **START generation** | "Send a START now!" |
| `CR1.STOP` (bit 9) | **STOP generation** | "Send a STOP now!" |
| `CR1.ACK` (bit 10) | **ACKnowledge enable** | "Reply ACK when I receive a byte" |
| `CR1.SWRST` (bit 15) | **SoftWare ReSeT** | Reset I2C (fresh start) |
| `CR2` | **Control Register 2** | Value 16 = "your clock is 16 MHz" |
| `CCR` | **Clock Control Register** | Value 80 = "run at 100 kHz" |
| `TRISE` | **T-RISE (maximum Rise time) Register** | Value 17 = timing safety value |
| `OAR1` | **Own Address Register 1** | **Slave only:** "my address is 0x30" |
| `DR` | **Data Register** | **The mailbox.** Write = send, read = receive |

### Status registers (the HARDWARE writes them, you just watch)

| Register | Full form |
|---|---|
| `SR1` | **Status Register 1** |
| `SR2` | **Status Register 2** |

Think of the flags in these registers as **notification lights**:

| Flag | Full form | Light ON means… |
|---|---|---|
| `SB` | **Start Bit** | ✓ START was sent |
| `ADDR` | **ADDRess sent (master) / ADDRess matched (slave)** | ✓ The slave said "yes, I'm here" (master side), or "someone called my address" (slave side) |
| `AF` | **Acknowledge Failure** | ✗ Nobody answered (NACK) |
| `TXE` | **Transmit data register Empty** | ✓ Mailbox empty, you can put the next byte in |
| `RXNE` | **Receive data register Not Empty** | ✓ Mailbox full, a byte arrived, read it! |
| `BTF` | **Byte Transfer Finished** | ✓ Byte fully sent |
| `STOPF` | **STOP detection Flag** | ✓ STOP received (slave side) |
| `TRA` (in SR2) | **TRAnsmitter / receiver** | Slave side: 1 = "master wants to READ from me" |

### Setup code (the same pattern on both boards)
```c
I2C1->CR1 |=  SWRST;   // reset
I2C1->CR1 &= ~SWRST;   // release reset
I2C1->CR2   = 16;      // clock speed = 16 MHz
I2C1->CCR   = 80;      // 100 kHz
I2C1->TRISE = 17;      // rise time limit
I2C1->OAR1  = (1<<14) | (0x30 << 1);  // SLAVE ONLY: my address = 0x30
I2C1->CR1 |= PE;       // turn I2C ON
I2C1->CR1 |= ACK;      // turn ACK ON (must come AFTER PE)
```

Where the numbers come from:
- `CCR = 16 MHz / (2 × 100 kHz) = 80`
- `TRISE = (16 MHz × 1000 ns) + 1 = 16 + 1 = 17`

---

## 6. How a Transfer Works: "Give an Order, Wait for the Light"

Every I2C step follows the same pattern:

```
1. YOU set a bit in CR1 or write DR     (give an order)
2. WAIT until a flag in SR1 turns ON    (hardware says "done")
3. Go to the next step
```

The address byte is the slave address shifted left by 1, plus the **R/W (Read/Write)** bit:
- `0x30 << 1 | 0` = **0x60** → Write
- `0x30 << 1 | 1` = **0x61** → Read

### Master sends a byte (WRITE), like making a phone call

```
YOU DO                           WAIT FOR LIGHT       MEANING
──────                           ──────────────       ───────
CR1 |= START                →    SB = 1               "line is open"
DR = 0x60 (address + write) →    ADDR = 1             "slave picked up"
read SR1, read SR2          →    (clears ADDR)        "ok, continue"
                                 TXE = 1              "mailbox empty"
DR = 0x01 (the data)        →    BTF = 1              "data delivered"
CR1 |= STOP                 →    done                 "hang up"
```

### Master reads a byte (READ)

```
CR1 |= START                →    SB = 1
DR = 0x61 (address + read)  →    ADDR = 1
CR1 &= ~ACK                      ("I only want 1 byte, then I'll say NACK")
read SR1, read SR2               (clears ADDR, slave starts sending)
CR1 |= STOP                      ("hang up after this byte")
                                 RXNE = 1             "byte arrived"
data = DR                        (take it out of the mailbox)
CR1 |= ACK                       (turn ACK back on for next time)
```

### Slave side: it just waits for its name to be called

```c
if (SR1 & ADDR) {               // "someone called my address!"
    read SR1, read SR2;         // clear ADDR
    if (SR2 had TRA = 1) {      // master wants to READ
        wait TXE;  DR = button_state;   // put the answer in the mailbox
        wait AF;   clear AF;            // master said "enough" (NACK)
    } else {                    // master wants to WRITE
        wait RXNE; cmd = DR;            // take the byte out
        wait STOPF; clear it;           // master hung up
        if cmd == 1 → blink LED
        if cmd == 0 → LED ON
    }
}
```

---

## 7. How Flags Get Cleared (Reset)

Some status lights don't switch off by themselves. You must clear them in a specific way:

| Flag | How to clear it |
|---|---|
| `SB` (Start Bit) | Write the address to `DR` |
| `ADDR` (Address sent/matched) | **Read SR1, then read SR2** |
| `TXE` (Transmit Empty) | Write a byte to `DR` |
| `RXNE` (Receive Not Empty) | Read `DR` |
| `AF` (Acknowledge Failure) | `SR1 &= ~AF` (write 0 to it) |
| `STOPF` (STOP Flag) | Read SR1, then write to CR1 |

---

## 8. The Delay Timer (SysTick)

**SysTick = System Tick timer**, built into every ARM Cortex-M chip.

| Register / Bit | Full form | Job |
|---|---|---|
| `CTRL` | **ConTRoL and status register** | Start/stop the timer |
| `CTRL.ENABLE` | Enable | Starts counting |
| `CTRL.CLKSOURCE` | **CLocK SOURCE** | Use the main 16 MHz clock |
| `CTRL.COUNTFLAG` | **COUNT FLAG** | Becomes 1 each time the count reaches 0 |
| `LOAD` | **reLOAD value register** | Where counting starts from |
| `VAL` | **current VALue register** | The current count |

```c
LOAD = 16000 - 1;   // 16000 ticks at 16 MHz = 1 ms
VAL  = 0;           // start fresh
CTRL = ENABLE | CLKSOURCE;
// wait for COUNTFLAG 'ms' times → 'ms' milliseconds have passed
```

---

## 9. The Whole Program in 5 Lines (for the Exam)

> 1. **RCC** wakes up GPIO and I2C.
> 2. **GPIO registers** (MODER, OTYPER, PUPDR, AFR) set up the LED, the button, and the I2C pins.
> 3. **I2C registers** (CR2, CCR, TRISE, OAR1, then PE and ACK in CR1) set the speed and the slave address, then turn I2C on.
> 4. To send or receive, the code **sets bits in CR1 / writes DR**, then **waits for flags in SR1** (SB → ADDR → TXE/RXNE → BTF), then sends STOP.
> 5. The master **writes** to tell the slave about its button, and **reads** (every 20 ms) to learn about the slave's button. Each board turns its LED on, off, or blinking based on the result.

---

## 10. Full Forms (Quick Glossary)

### General
| Short | Full form |
|---|---|
| I2C | Inter-Integrated Circuit |
| SCL | Serial CLock line |
| SDA | Serial DAta line |
| ACK | ACKnowledge |
| NACK | Not ACKnowledge |
| R/W | Read / Write bit |
| MSB | Most Significant Bit |
| GPIO | General Purpose Input/Output |
| LED | Light Emitting Diode |
| HSI | High-Speed Internal oscillator (16 MHz) |
| MHz / kHz | MegaHertz / kiloHertz |
| AF4 | Alternate Function 4 |
| PA5 / PB8 / PC13 | Port A pin 5 / Port B pin 8 / Port C pin 13 |
| STM32 | ST Microelectronics 32-bit microcontroller |
| ARM | Advanced RISC Machine |

### Clock (RCC)
| Short | Full form |
|---|---|
| RCC | Reset and Clock Control |
| AHB | Advanced High-performance Bus |
| APB | Advanced Peripheral Bus |
| AHB1ENR | AHB1 peripheral clock ENable Register |
| APB1ENR | APB1 peripheral clock ENable Register |

### GPIO registers
| Short | Full form |
|---|---|
| MODER | MODE Register |
| OTYPER | Output TYPE Register |
| OSPEEDR | Output SPEED Register |
| PUPDR | Pull-Up / Pull-Down Register |
| IDR | Input Data Register |
| ODR | Output Data Register |
| BSRR | Bit Set / Reset Register |
| LCKR | LoCK Register |
| AFR | Alternate Function Register (AFR[0] = Low, AFR[1] = High) |

### I2C registers
| Short | Full form |
|---|---|
| CR1 / CR2 | Control Register 1 / 2 |
| OAR1 / OAR2 | Own Address Register 1 / 2 |
| DR | Data Register |
| SR1 / SR2 | Status Register 1 / 2 |
| CCR | Clock Control Register |
| TRISE | T-RISE (maximum Rise time) Register |
| FLTR | FiLTer Register |

### I2C bits / flags
| Short | Full form |
|---|---|
| PE | Peripheral Enable |
| START | START generation |
| STOP | STOP generation |
| ACK | ACKnowledge enable |
| SWRST | SoftWare ReSeT |
| SB | Start Bit (START condition generated) |
| ADDR | ADDRess sent (master) / ADDRess matched (slave) |
| BTF | Byte Transfer Finished |
| TXE | Transmit data register Empty |
| RXNE | Receive data register Not Empty |
| AF | Acknowledge Failure |
| STOPF | STOP detection Flag |
| TRA | TRAnsmitter / receiver |

### SysTick
| Short | Full form |
|---|---|
| SysTick | System Tick timer |
| CTRL | ConTRoL and status register |
| LOAD | reLOAD value register |
| VAL | current VALue register |
| CALIB | CALIBration value register |
| CLKSOURCE | CLocK SOURCE |
| COUNTFLAG | COUNT FLAG |
