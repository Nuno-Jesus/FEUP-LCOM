#ifndef I8042_H
#define I8042_H

#include <lcom/lcf.h>

#define KBD_IRQ 1 

//Port adresses
#define KBC_STAT_REG  0x64 
#define KBC_CMD_REG   0x64
#define KBC_OUT_BUF   0x60

//Status register bits
#define OBF     BIT(0)
#define IBF     BIT(1)     
#define SYS     BIT(2) // Not used
#define A2      BIT(3) // Not used
#define INH     BIT(4) // Not used
#define AUX     BIT(5) // Not used
#define TO_ERR  BIT(6)
#define PAR_ERR BIT(7)

#define BREAKCODE BIT(7)

//Scancodes
#define FIRST_BYTE_OUT_OF_TWO 0xE0
#define ESC 0x81


#endif 
