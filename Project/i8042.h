#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/******************************* KEYBOARD *******************************/

//Keyboard IRQ line
#define KBD_IRQ 1 

//Port adresses
#define KBC_STAT_REG  0x64 
#define KBC_CMD_REG   0x64
#define KBC_OUT_BUF   0x60

//Status register bits
#define OBF       BIT(0)
#define IBF       BIT(1)     
#define SYS       BIT(2) // Not used
#define A2        BIT(3) // Not used
#define INH       BIT(4) // Not used
#define AUX       BIT(5) // Not used
#define TO_ERR    BIT(6)
#define PAR_ERR   BIT(7)

#define BREAKCODE BIT(7)

//Scancodes
#define FIRST_BYTE_OUT_OF_TWO 0xE0
#define ESC                   0x81

/******************************** MOUSE ********************************/

//Mouse IRQ line
#define MOUSE_IRQ 12

//To distinguish the first byte of a packet
#define FIRST_BYTE BIT(3)

//Mouse packet - first byte
#define MOUSE_Y_OVFL  BIT(7)
#define MOUSE_X_OVFL  BIT(6)
#define MSB_Y_DELTA   BIT(5)
#define MSB_X_DELTA   BIT(4)
#define MIDDLE_BUTTON BIT(2)
#define RIGHT_BUTTON  BIT(1)
#define LEFT_BUTTON   BIT(0)

//Mouse-related KBC commands (written to port 0x60 / response read from the OUT BUF)
#define READ_CMDB               0x20
#define WRITE_CMBD              0x60
#define DISABLE_MOUSE           0xA7
#define ENABLE_MOUSE            0xA8
#define WRITE_BYTE_MOUSE        0xD4

//Enable / Disable mouse data reporting
#define ENABLE_DATA_REPORTING   0xF4
#define DISABLE_DATA_REPORTING  0xF5

//Mouse controller byte response
#define ACK   0xFA    //Acknowledged
#define NACK  0xFE    //Invalid byte
#define ERROR 0xFC    //Second consecutive invalid byte

#endif
