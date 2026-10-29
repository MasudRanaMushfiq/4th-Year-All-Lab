/**
 * I2C TWO-WAY SWITCH — MASTER — Pure Bare Metal — NUCLEO-F446RE
 * Both boards have a button (PC13) and an LED (PA5). I2C is master-driven,
 * so THIS board drives both directions:
 *
 *   MY button pressed    → WRITE 'S' to slave 0x28 → slave blinks 5× → I little-blink.
 *   SLAVE button pressed → I find out by READING the slave's button byte
 *                          → I blink 5× → WRITE 'K' so the slave little-blinks.
 * Idle: both dark. Guard counters => a missing slave can never hang this.
 *
 * WIRING (2 wires + GND + pull-ups): PB8(SCL)↔PB8, PB9(SDA)↔PB9, 4.7kΩ each to 3.3V, GND↔GND
 * LED: PA5   BUTTON: PC13   LINK: I2C1 @ 100kHz
 */

#define SLAVE_ADDR        0x28   // Slave's 7-bit I2C address: hexadecimal 0x28 
#define BLINK_MS          200    // LED on/off duration: 200 ms each 
#define SLAVE_BLINK_WAIT  2500   // Master waits 2500 ms (2.5 seconds) for the slave's 5 blinks 
#define GUARD             200000 // Maximum number of polling iterations before a timeout 

// Memory-mapped hardware registers 
// This memory can change at any time for reasons you can't see — never cache it
// unsigned is the type that says "these are 32 plain bits, no sign"
// Star #1 (the dereference * at the very front): "Go fetch the value at this address."
// Star #2 (the pointer-type * inside the cast): "This type is a pointer to unsigned int." 

// RCC is the Reset and Clock Control 
// AHB1ENR — Advanced High-performance Bus 1 ENable Register (AHB is the fast system bus — the CPU's main highway.)
// APB = Advanced Peripheral Bus. (APB is the slow peripheral bus — a side street for simple devices.)

#define RCC_AHB1ENR   (*(volatile unsigned int *)0x40023830) // RCC register that enables clocks on AHB1 (Enables GPIO peripheral clocks)
#define RCC_APB1ENR   (*(volatile unsigned int *)0x40023840) // The RCC register that enables clocks on the APB1 bus. (Enables the I2C1 peripheral clock)
#define GPIOA_MODER   (*(volatile unsigned int *)0x40020000) // (Configures The Mode Register for GPIO Port A)
#define GPIOA_ODR     (*(volatile unsigned int *)0x40020014) // (Controls Output Data Register levels on GPIO port A)
#define GPIOB_MODER   (*(volatile unsigned int *)0x40020400) // (Configures GPIOB pin modes)
#define GPIOB_OTYPER  (*(volatile unsigned int *)0x40020404) // GPIO B Output TYpe Register. (Set the output type as push-pull or open-drain)
#define GPIOB_PUPDR   (*(volatile unsigned int *)0x4002040C) // When nothing is actively driving this pin, what voltage should it settle to? (Configures GPIOB pull-up/pull-down resistors)
#define GPIOB_AFRH    (*(volatile unsigned int *)0x40020424) // Alternate Function Register High (Selects alternate functions for GPIOB pins 8–15)
#define GPIOC_PUPDR   (*(volatile unsigned int *)0x4002080C) // (Configures GPIOC pull-up/pull-down resistors)
#define GPIOC_IDR     (*(volatile unsigned int *)0x40020810) // Input Data Register.(Reads GPIOC input levels)
#define I2C1_CR1      (*(volatile unsigned int *)0x40005400) // control register for the I2C1 peripheral —
//  it contains the master enable, START/STOP generation, ACK control, and various interrupt enables. (Controls I2C operation)
#define I2C1_CR2      (*(volatile unsigned int *)0x40005404) // Configures I2C peripheral clock frequency
#define I2C1_DR       (*(volatile unsigned int *)0x40005410) // I2C 1 Data Register. (Holds transmitted or received data)
#define I2C1_SR1      (*(volatile unsigned int *)0x40005414) // (Reports I2C status flags)
#define I2C1_SR2      (*(volatile unsigned int *)0x40005418) // I2C 1 Status Register 1. (Reports additional I2C status, including bus ownership)
#define I2C1_CCR      (*(volatile unsigned int *)0x4000541C) // I2C 1 Clock Control Register. (Configures I2C clock timing)
#define I2C1_TRISE    (*(volatile unsigned int *)0x40005420) // I2C 1 TRise Time Register.(Configures the I2C max rise-time limit)

// void SystemInit(void) __attribute__((weak));
// void SystemInit(void) { }
// void __libc_init_array(void) __attribute__((weak)); 
// void __libc_init_array(void) { }

static void delay_ms(unsigned int ms) {                   // A delay function
    for (unsigned int m = 0; m < ms; m++)
        for (volatile unsigned int i = 0; i < 1300; i++);
}

static int pressed(void) { return (GPIOC_IDR & (1 << 13)) == 0; } // This checks whether the master board's button on PC13 is pressed or not

static void blink(int times) {                             // This function blinks the master LED on PA5 a specified number of times.
    for (int i = 0; i < times; i++) {
        GPIOA_ODR |=  (1 << 5);  delay_ms(BLINK_MS);  // turning PA5 HIGH.
        GPIOA_ODR &= ~(1 << 5);  delay_ms(BLINK_MS);  // turning PA5 LOW.
    }
}

static void little_blink(void) {                   // This is similar to blink(), but it has a fixed number of blinks and a shorter duration.
    for (int i = 0; i < 2; i++) {
        GPIOA_ODR |=  (1 << 5);  delay_ms(120);
        GPIOA_ODR &= ~(1 << 5);  delay_ms(120);
    }
}

/* Write one byte to the slave. 1 = ACKed, 0 = no slave / error. */
static int i2c_write(unsigned char addr, unsigned char data) { // this function can be called only from within this C source file
    unsigned int g; // Creates an unsigned integer variable named g
    g = GUARD; 
    // The semicolon means the loop has an empty body.
    while ((I2C1_SR2 & (1 << 1)) && --g);  // This checks whether the I2C bus is busy. I2C1_SR2 reads the second I2C status register. 
    if (!g) return 0; // If the bus remains busy until the counter expires, the function returns 0.
    I2C1_CR1 |= (1 << 8); // The peripheral generates a START condition on the bus when it is able to do so. Bit 8 is the START bit.
    g = GUARD; 
    while (!(I2C1_SR1 & (1 << 0)) && --g); // This checks the SB flag, which is bit 0 of I2C1_SR1. Wait for the START condition to be detected 
    if (!g) { I2C1_CR1 |= (1<<9); return 0; } // Set bit 9 (STOP) in I2C1_CR1. The STOP request attempts to end the transaction.
    I2C1_DR = (addr << 1) | 0;               // Shift a bit left to add R/W bit and then make write as set 0
    g = GUARD; 

    // Bit 1: ADDR, indicating that the address phase has completed.
    // Bit 10: AF, indicating acknowledge failure.
    // This selects both bits, so the loop can stop when either flag becomes set.
    while (!(I2C1_SR1 & ((1<<1)|(1<<10))) && --g);
    if (I2C1_SR1 & (1 << 10)) {   // Checks whether the AF flag is set. 
        I2C1_SR1 &= ~(1<<10);     // Attempts to clear the AF flag by writing zero to it while preserving the other bits. 
        I2C1_CR1 |= (1<<9);       // Requests a STOP condition.
        return 0; 
    }
    if (!(I2C1_SR1 & (1 << 1))) { // This checks whether the ADDR flag is set. It indicates that the address phase has completed successfully.
        I2C1_CR1 |= (1<<9); 
        return 0; 
    }
    (void)I2C1_SR1; (void)I2C1_SR2; // Clear the address flag 
    g = GUARD; 
    // after the slave address has been handled, the master waits until the peripheral is ready to accept the actual data byte.
    while (!(I2C1_SR1 & (1 << 7)) && --g);
    I2C1_DR = data; // This writes the requested byte into the I2C data register.
    g = GUARD; 
    while (!(I2C1_SR1 & (1 << 2)) && --g); // Bit 2 is the BTF flag, meaning Byte Transfer Finished.
    I2C1_CR1 |= (1 << 9); // Generate the STOP condition, Bit 9 is the STOP bit.
    return 1; // Returns 1 to the calling code. reports that the I2C write succeeded.
}

/* Read one byte from the slave into *out. 1 = ok, 0 = no slave / error. */
static int i2c_read(unsigned char addr, unsigned char *out) { // *out — a pointer to a variable where the received byte will be stored.
    unsigned int g;
    I2C1_CR1 |= (1 << 10); // Enable ACK, Bit 10 is the ACK bit.
    g = GUARD; 
    while ((I2C1_SR2 & (1 << 1)) && --g); // Bit 1 is the BUSY flag. I2C1_SR2 & (1 << 1) checks whether the bus is busy.
    if (!g) return 0;
    I2C1_CR1 |= (1 << 8); // Generate START
    g = GUARD;  
    while (!(I2C1_SR1 & (1 << 0)) && --g);  // Wait for the START condition to be generated
    if (!g) { I2C1_CR1 |= (1<<9); return 0; } // Bit 9 is the STOP bit. Setting it requests a STOP condition
    I2C1_DR = (addr << 1) | 1; // Send the slave address with the read bit
    g = GUARD; 
    while (!(I2C1_SR1 & ((1<<1)|(1<<10))) && --g); // Wait for address acknowledgment or an error 
    if (I2C1_SR1 & (1 << 10)) { // The if checks whether the AF flag is set.
        I2C1_SR1 &= ~(1<<10); // The next line attempts to clear the AF flag.
        I2C1_CR1 |= (1<<9); // The third line requests STOP.
        return 0; // return 0; reports failure.
    }
    if (!(I2C1_SR1 & (1 << 1))) { I2C1_CR1 |= (1<<9); return 0; } // Check whether the address was acknowledged, if ACK then generate STOp signal 
    I2C1_CR1 &= ~(1 << 10);                            /* NACK for the single byte */
    (void)I2C1_SR1; (void)I2C1_SR2;
    I2C1_CR1 |= (1 << 9); // Request STOP
    g = GUARD; 
    while (!(I2C1_SR1 & (1 << 6)) && --g); // Bit 6 is RxNE, meaning Receive Data Register Not Empty.
    if (!g) return 0;
    *out = (unsigned char)I2C1_DR; // Store the received byte
    return 1;
}

int main(void)
{
    RCC_AHB1ENR |= (1 << 0) | (1 << 1) | (1 << 2);   /* Enable GPIOA + GPIOB + GPIOC */
    RCC_APB1ENR |= (1 << 21);                        /* Enable the I2C1 clock */

    GPIOA_MODER &= ~(3 << 10);  GPIOA_MODER |= (1 << 10);        /* First clear and then Configure PA5 as an output */
    GPIOB_MODER  &= ~((3 << 16) | (3 << 18));                    /* PB8/9 AF4(alternate function 4) OD PU */
    GPIOB_MODER  |=  ((2 << 16) | (2 << 18));        // The first line clears both pins' mode bits. The second line sets each pair to binary 10.
    GPIOB_OTYPER |=  (1 << 8) | (1 << 9);            // Configure PB8 and PB9 as open-drain
    GPIOB_PUPDR  &= ~((3 << 16) | (3 << 18));        // Enable pull-up resistors for PB8 and PB9
    GPIOB_PUPDR  |=  ((1 << 16) | (1 << 18));
    GPIOB_AFRH   &= ~((0xF << 0) | (0xF << 4));      // Select alternate function AF4 for PB8 and PB9
    GPIOB_AFRH   |=  ((4 << 0) | (4 << 4));
    GPIOC_PUPDR  &= ~(3 << 26);  GPIOC_PUPDR |= (1 << 26); /* PC13 pull-up , PC13 uses bits 27 and 26 in GPIOC_PUPDR, because (13 times 2 = 26.*/

    I2C1_CR1 |= (1 << 15);  I2C1_CR1 &= ~(1 << 15); // Reset the I2C1 peripheral, Bit 15 is SWRST (software reset).
    I2C1_CR2 = 16;   // Set the I2C peripheral clock frequency
    I2C1_CCR = 80;   // Set the I2C clock speed
    I2C1_TRISE = 17; // Configure the maximum SCL rise time
    I2C1_CR1 |= (1 << 0); // Enable the I2C peripheral

    int prev = 0, prevB = 0;
    while (1) {
        /* ---- MY button → slave blinks, then I little-blink ---- */
        int now = pressed();
        if (now && !prev) { // This technique is called edge detection. Here, !prev means “the previous state was not pressed.” 
            delay_ms(20);
            if (pressed() && i2c_write(SLAVE_ADDR, 'S')) {
                delay_ms(SLAVE_BLINK_WAIT);        /* let the slave finish */
                little_blink();                    /* my ack */
            }
        }
        prev = now; // Saves the current button state for the next loop iteration.

        /* ---- poll the SLAVE's button ---- */
        unsigned char r = 0;
        if (i2c_read(SLAVE_ADDR, &r)) {
            int nowB = (r == 'B');        // Check whether the slave reports its button as pressed, n your protocol, 'B' means that the slave's button is pressed.
            if (nowB && !prevB) {                  /* slave just pressed as new */
                blink(5);                          /* my LED blinks      */
                i2c_write(SLAVE_ADDR, 'K');        /* ack → slave little-blinks */
            }
            prevB = nowB;
        }

        delay_ms(10);
    }
}


