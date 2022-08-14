#include "mouse.h"

int hook_id;
uint8_t scancode;
uint8_t status;
struct mouse_ev *event;

#define MAX_ATTEMPTS 3

bool (buffer_is_full)(uint8_t buffer){
  uint8_t status;

  //Reads the status register to get the KBC status
  if(utils_sys_inb(KBC_STAT_REG, &status) != OK)
    return false;

  //Checks for a full buffer (either the output or input buffer)
  if(status & buffer){
    
    //Aborts if there's a parity or timeout error
    if((status & (PAR_ERR | TO_ERR)) != OK)
      return false;

    else 
      return true;
  }

  //If the buffer isn't even full, returns false
  return false;
}

int (mouse_subscribe_int)(uint8_t *bit_no){
  hook_id = MOUSE_IRQ;
  *bit_no = hook_id;
  if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id) != OK) 
    return 1;

  return OK;
}

int (mouse_unsubscribe_int)() {
  uint8_t temp;

  if (sys_irqrmpolicy(&hook_id) != OK) 
    return 1;

  //Reads the status from the status register
  if (utils_sys_inb(KBC_CMD_REG, &status) != OK) 
    return 1;

  //To check for a lost byte
  if (status & OBF)
    if (utils_sys_inb(KBC_OUT_BUF, &temp) != OK) 
      return 1;

  return OK;
}

void (mouse_ih)(){
  while(1){
    if(buffer_is_full(OBF)){
      if (utils_sys_inb(KBC_OUT_BUF, &scancode) != OK) 
        scancode = 0x00;

      break;
    }
  }
}

void (mouse_assemble_packet)(struct packet * pack){
  pack->lb = pack->bytes[0] & LEFT_BUTTON;
  pack->rb = pack->bytes[0] & RIGHT_BUTTON;
  pack->mb = pack->bytes[0] & MIDDLE_BUTTON;

  pack->x_ov = pack->bytes[0] & MOUSE_X_OVFL;
  pack->y_ov = pack->bytes[0] & MOUSE_Y_OVFL;

  if(pack->bytes[0] & MSB_X_DELTA)
    pack->delta_x = convert_2_complement(pack->bytes[1]);
  
  else
    pack->delta_x = pack->bytes[1];

  if(pack->bytes[0] & MSB_Y_DELTA)
    pack->delta_y = convert_2_complement(pack->bytes[2]);

  else
    pack->delta_y = pack->bytes[2];

}

int (mouse_enable_data_report)(){
  uint8_t response;

  for(int error = 0; error < MAX_ATTEMPTS; ){
    //Issues the 0xD4 KBC command to allow writing a byte to the mouse
    if (sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE) != OK) 
      return 1;

    //Sends the byte to be written
    if (mouse_write_command(ENABLE_DATA_REPORTING, &response) != OK) 
      return 1;

    if (response == ACK) 
      return OK;
    
    else if (response == NACK) 
      error++;
    
    else if (response == ERROR) 
      return 1;
  } 

  return 1;
}


int (mouse_disable_data_report)(){
  uint8_t response;

  for(int error = 0; error < MAX_ATTEMPTS; ){
    //Issues the 0xD4 KBC command to allow writing a byte to the mouse
    if (sys_outb(KBC_CMD_REG, WRITE_BYTE_MOUSE) != OK) 
      return 1;

    //Sends the byte to be written
    if (mouse_write_command(DISABLE_DATA_REPORTING, &response) != OK) 
      return 1;

    if (response == ACK) 
      return OK;
    
    else if (response == NACK) 
      error++;
    
    else if (response == ERROR) 
      return 1;
  } 

  return 1;
}

int(mouse_write_command)(uint32_t command, uint8_t *response){
  int attempts = 0;

  while (attempts < MAX_ATTEMPTS) {
    //If the input buffer isn't full
    if (!buffer_is_full(IBF)){
      //Writes cmd on 0x60
      if (sys_outb(WRITE_CMBD, command) != OK) 
        return 1;

      //Gets the response byte from the output buffer
      if (utils_sys_inb(KBC_OUT_BUF, response) != OK) 
        return 1;

      return OK;
    }
    
    attempts++;
  }

  return 1;
}



struct mouse_ev* (mouse_events)(struct packet *pack){
  //These variables need to be static, once the old status would always be false
  static bool hasPressedLeftButton = false;
  static bool hasPressedRightButton = false;
  static bool hasPressedMiddleButton = false;

  event->delta_x = pack->delta_x;
  event->delta_y = pack->delta_y;

  //Hadnt press the left button before, but the mouse packet detected a push
  if (!hasPressedLeftButton && pack->lb && !hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedLeftButton = true;
    event->type = LB_PRESSED;
  }

  //Left button was pressed before and the mouse packet detected a release
  else if (hasPressedLeftButton && !pack->lb && !hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedLeftButton = false;
    event->type = LB_RELEASED;
  }

  //Hadnt press the right button before, but the mouse packet detected a push
  else if (!hasPressedLeftButton && pack->lb && !hasPressedRightButton && pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedRightButton = true;
    event->type = RB_PRESSED;
  }

  //Right button was pressed before and the mouse packet detected a release
  else if (!hasPressedLeftButton && pack->lb && hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedRightButton = false;
    event->type = RB_RELEASED;
  }

  //Hadnt press the middle button before, but the mouse packet detected a push
  else if (!hasPressedLeftButton && pack->lb && !hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && pack->mb){
    hasPressedMiddleButton = true;
    event->type = BUTTON_EV;
  }

  //Middle button was pressed before and the mouse packet detected a release
  else if(!hasPressedLeftButton && pack->lb && !hasPressedRightButton && !pack->rb && hasPressedMiddleButton && !pack->mb){
    hasPressedMiddleButton = false;
    event->type = BUTTON_EV;
  }

  //No button pressed, only mouse displacement
  else 
    event->type = MOUSE_MOV;

  return event;
}
