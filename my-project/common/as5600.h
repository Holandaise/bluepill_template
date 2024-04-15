#ifndef _AS5600_H
#define _AS5600_H

// AS5600 returns 0 when reading from invalid addresses.
// The internal address pointer is incremented after each byte read.
// NOTE: the ANGLE and RAW ANGLE addresses are not auto-incremented
// as long as the high byte was addressed.

#define SLAVE_ADDR 0x36
// Register addresses
enum AS5600_REGS{
    ZMCO,
    ZPOS,
    MPOS = 0x03,
    MANG = 0x05,
    CONF = 0x07,
    RAW_ANGLE = 0x0C,
    ANGLE = 0x0E,
    STATUS = 0x0B,
    AGC = 0x1A,
    MAGNITUDE = 0x1b,
    BURN = 0xFF,
};

// CONF register defines
# define PM 0
#define HYST 2
#define OUTS 4
#define PWMF 6
#define SF 8
#define FTH 10
#define WD 13

#endif