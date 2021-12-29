#include "kbd.h"
#include "i8042.h"

uint8_t scancode, count_in;
uint8_t stat;

int hook_id_keyboard;

int (kbd_subscribe_int)(uint8_t *bit_no) {
  hook_id_keyboard = 1;
  *bit_no = hook_id_keyboard;
  if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_keyboard) != OK) 
    return 1;

  return 0;
}

int (kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_keyboard) != OK) 
    return 1;

  return 0;
}

void (kbc_ih)(){
  while(1){
    //Reads the status register looking for possible errors
    if (utils_sys_inb(KBC_STAT_REG, &stat) != OK) 
      scancode = 0x00;
    
    //If the output buffer is full
    if (stat & OBF){
      //Reads the scancode on the output buffer
      if (utils_sys_inb(KBC_OUT_BUF, &scancode) != OK)
        scancode = 0x00;
      
      //And checks for parity or timeout errors
      if ((stat & (PAR_ERR | TO_ERR)) != OK)
        scancode = 0x00;

      break;
    }
  }
}
