# I2C Protocol — Exam-Ready Notes

> Based on the STM32F446RE bare-metal lab (`i2c_master.c` / `i2c_slave.c`).

---

## 1. What is I2C?

- **I2C** = **I**nter-**I**ntegrated **C**ircuit (pronounced "I-squared-C"). Invented by Philips (now NXP).
- **Synchronous, serial, half-duplex, multi-master / multi-slave** bus.
- Uses only **2 wires**:
  - **SCL**: Serial Clock, driven by the master
  - **SDA**: Serial Data, bidirectional
- Every device on the bus has a unique **7-bit address** (10-bit addressing also exists).
- Up to **127 devices** with 7-bit addressing (some addresses are reserved, so in practice ~112).

## 2. Key Hardware Facts

| Point | Detail |
|---|---|
| Output type | **Open-drain**: a device can only pull a line LOW, never drive it HIGH |
| Pull-ups | **External pull-up resistors are required** (typically **4.7 kΩ** to VCC) |
| Idle state | Both SCL and SDA are **HIGH** |
| Common ground | All devices must share **GND** |
| Wired-AND | If any device pulls the line LOW, the line reads LOW (this is what makes arbitration work) |

**Why open-drain?** It avoids short circuits when two devices drive the line at once, and it makes clock stretching and arbitration possible.

## 3. Speed Modes

| Mode | Speed |
|---|---|
| Standard | **100 kbps** (used in this lab) |
| Fast | 400 kbps |
| Fast-mode Plus | 1 Mbps |
| High-speed | 3.4 Mbps |
| Ultra-fast | 5 Mbps (unidirectional) |

## 4. Bus Conditions (Most Asked!)

| Condition | Rule |
|---|---|
| **START (S)** | SDA goes **HIGH → LOW** while SCL is **HIGH** |
| **STOP (P)** | SDA goes **LOW → HIGH** while SCL is **HIGH** |
| **Repeated START (Sr)** | A START without a STOP first, so the master keeps the bus |
| **Data valid** | SDA must stay **stable while SCL is HIGH** and may change only while SCL is LOW |
| **ACK** | Receiver pulls SDA **LOW** on the 9th clock |
| **NACK** | Receiver leaves SDA **HIGH** on the 9th clock |

```
         START                                         STOP
SCL  ‾‾‾‾‾‾‾\__/‾\__/‾\__ ... __/‾\__/‾\__/‾‾‾‾‾‾‾‾‾‾‾‾‾‾
SDA  ‾‾‾‾\____X===X===X== ... ==X===X_______/‾‾‾‾‾‾‾‾‾‾‾
           ↑ SDA falls while              ↑ SDA rises while
             SCL is HIGH                    SCL is HIGH
```

## 5. Frame Format

```
| S | 7-bit ADDRESS | R/W | ACK | 8-bit DATA | ACK | 8-bit DATA | ACK/NACK | P |
```

- **Address byte** = `(7-bit address << 1) | R/W`
  - **R/W = 0 → Write** (master → slave)
  - **R/W = 1 → Read** (slave → master)
- Data is sent **MSB first**.
- Each byte is **8 data bits + 1 ACK bit = 9 clocks**.

**Example from the lab:** `SLAVE_ADDR = 0x30`
- Write address byte = `(0x30 << 1) | 0` = **0x60**
- Read address byte = `(0x30 << 1) | 1` = **0x61**

## 6. Write vs. Read Transaction

**Master WRITE (master → slave):**
```
S → ADDR+W → [slave ACK] → DATA → [slave ACK] → ... → P
```

**Master READ (slave → master):**
```
S → ADDR+R → [slave ACK] → DATA → [master ACK] → ... → last DATA → [master NACK] → P
```
- When reading, the **master NACKs the last byte** to tell the slave to stop sending.

## 7. Important Concepts

- **Only the master can start a transaction.** A slave can never push data on its own.
  - To get slave → master data, the **master polls** the slave with READ transactions. The lab does this every ~20 ms to read the slave's button state.
- **Clock stretching**: a slave holds SCL LOW to make the master wait while it is busy.
- **Arbitration** (multi-master): when two masters transmit at once, the one that sends a **1** but reads back a **0** loses and backs off. No data is corrupted.
- **Address NACK**: no device answered, so either the address is wrong or the slave is not powered or running.

## 8. STM32 I2C Registers (Lab-Specific)

| Register / Bit | Purpose |
|---|---|
| `CR1.PE` | Peripheral enable |
| `CR1.START` / `CR1.STOP` | Generate a START / STOP condition |
| `CR1.ACK` | Enable ACK after each received byte (set it **after** PE) |
| `CR1.SWRST` | Software reset of the peripheral |
| `CR2` | APB1 clock frequency in MHz (`16`) |
| `CCR` | Clock control: `16 MHz / (2 × 100 kHz) = 80` |
| `TRISE` | Max rise time: `(16 MHz × 1000 ns) + 1 = 17` |
| `OAR1` | Slave's own address: `(addr << 1)`, and bit 14 must be kept at 1 |
| `SR1.SB` | START was generated |
| `SR1.ADDR` | Address sent and ACKed. **Clear it by reading SR1 then SR2** |
| `SR1.TXE` | Data register empty, ready to transmit |
| `SR1.RXNE` | Data register not empty, a byte was received |
| `SR1.BTF` | Byte transfer finished |
| `SR1.AF` | Acknowledge failure (NACK received) |
| `SR1.STOPF` | STOP detected (slave side) |
| `SR2.TRA` | Slave side: 1 = transmitter (master is reading) |

### GPIO setup for I2C (PB8 = SCL, PB9 = SDA)
- Mode = **Alternate Function (AF4)**
- Output type = **Open-drain**
- Speed = High, Pull-up enabled (external 4.7 kΩ still recommended)

### Master write sequence
1. Set `START` and wait for `SB`.
2. Write `(addr<<1)|0` to `DR` and wait for `ADDR` (or `AF` means NACK).
3. Read `SR1`, then `SR2`, to clear `ADDR`.
4. Wait for `TXE`, then write the data to `DR`.
5. Wait for `BTF`, then set `STOP`.

### Master single-byte read sequence
1. Set `START` and wait for `SB`.
2. Write `(addr<<1)|1` to `DR` and wait for `ADDR`.
3. **Clear `ACK`** (so the byte gets a NACK), clear `ADDR`, then **set `STOP`**.
4. Wait for `RXNE` and read `DR`.
5. Set `ACK` again for the next transfer.

## 9. I2C vs. SPI vs. UART

| Feature | I2C | SPI | UART |
|---|---|---|---|
| Wires | 2 (SCL, SDA) | 4 (SCK, MOSI, MISO, SS) | 2 (TX, RX) |
| Clock | Synchronous | Synchronous | Asynchronous |
| Duplex | Half | Full | Full |
| Devices | Multi-master, multi-slave | 1 master, many slaves | Point-to-point |
| Addressing | Address in the frame | Chip-select line | None |
| ACK | Yes | No | No (parity bit is optional) |
| Speed | Up to 3.4 Mbps | 10+ Mbps | Typically ≤ 1 Mbps |

## 10. Advantages / Disadvantages

**Advantages:** only 2 wires, many devices on one bus, built-in ACK, multi-master support.

**Disadvantages:** slower than SPI, half-duplex, needs pull-ups, short distances only (bus capacitance limit of **400 pF**), more complex protocol.

## 11. Common Faults (Debugging)

| Symptom | Likely cause |
|---|---|
| `SB` never sets (START timeout) | Missing pull-ups, bus stuck LOW, wiring or GND fault |
| `AF` set after the address | Wrong slave address, or the slave is not running |
| Bus hangs | Slave is holding SDA LOW. Send a STOP or reset the peripheral |

---

### Quick Memory Points
- **2 wires, open-drain, pull-ups, idle HIGH**
- **START: SDA ↓ while SCL HIGH. STOP: SDA ↑ while SCL HIGH**
- **Address byte = addr<<1 | R/W (0 = Write, 1 = Read)**
- **9 clocks per byte (8 data + ACK). ACK = LOW, NACK = HIGH**
- **The master always starts the transaction. The slave only responds.**
