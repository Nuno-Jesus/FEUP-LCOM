#include "mouse.h"

int hook_id_mouse;
uint8_t scancode;
uint8_t status;

//From "graphics_card.h"
extern unsigned int hres;
extern unsigned int vres;

//From "Menu.h"
extern xpm_image_t background_frame;

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
  hook_id_mouse = MOUSE_IRQ;
  *bit_no = hook_id_mouse;
  if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_mouse) != OK) 
    return 1;

  return OK;
}

int (mouse_unsubscribe_int)() {
  uint8_t temp;

  if (sys_irqrmpolicy(&hook_id_mouse) != OK) 
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



struct mouse_ev (mouse_events)(struct packet *pack){
  //These variables need to be static, once the old status would always be false
  static bool hasPressedLeftButton = false;
  static bool hasPressedRightButton = false;
  static bool hasPressedMiddleButton = false;

  struct mouse_ev event;
  
  event.delta_x = pack->delta_x;
  event.delta_y = pack->delta_y;

  //Hadnt press the left button before, but the mouse packet detected a push
  if (!hasPressedLeftButton && pack->lb && !hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedLeftButton = true;
    event.type = LB_PRESSED;
  }

  //Left button was pressed before and the mouse packet detected a release
  else if (hasPressedLeftButton && !pack->lb && !hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedLeftButton = false;
    event.type = LB_RELEASED;
  }

  //Hadnt press the right button before, but the mouse packet detected a push
  else if (!hasPressedLeftButton && !pack->lb && !hasPressedRightButton && pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedRightButton = true;
    event.type = RB_PRESSED;
  }

  //Right button was pressed before and the mouse packet detected a release
  else if (!hasPressedLeftButton && !pack->lb && hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && !pack->mb){
    hasPressedRightButton = false;
    event.type = RB_RELEASED;
  }

  //Hadnt press the middle button before, but the mouse packet detected a push
  else if (!hasPressedLeftButton && !pack->lb && !hasPressedRightButton && !pack->rb && !hasPressedMiddleButton && pack->mb){
    hasPressedMiddleButton = true;
    event.type = BUTTON_EV;
  }

  //Middle button was pressed before and the mouse packet detected a release
  else if(!hasPressedLeftButton && !pack->lb && !hasPressedRightButton && !pack->rb && hasPressedMiddleButton && !pack->mb){
    hasPressedMiddleButton = false;
    event.type = BUTTON_EV;
  }

  //No button pressed, only mouse displacement
  else{
    event.type = MOUSE_MOV;
  }

  return event;
}


/****************************** CURSOR ******************************/
Cursor *cursor;

void (new_cursor)(uint16_t x, uint16_t y){
  cursor = (Cursor*) malloc(sizeof(Cursor));
  if(cursor == NULL)
    return;

  cursor->x = x;
  cursor->y = y;
  cursor->isAboveButton = false;
  cursor->isAbovePiece = false;

  xpm_load(cursor_xpm, XPM_8_8_8_8, &(cursor->img));
  xpm_load(select_cursor_xpm, XPM_8_8_8_8, &(cursor->select_img));
}

void (retrieve_cursor_coordinates)(uint16_t* x, uint16_t* y){
  *x = cursor->x;
  *y = cursor->y;
}

void (draw_cursor_frame)(){
  xpm_image_t bitmap;
  uint32_t *bytes;

  if(cursor->isAboveButton || cursor->isAbovePiece)
    bitmap = cursor->select_img;

  else
    bitmap = cursor->img;

  bytes = (uint32_t*) bitmap.bytes;
  for(uint16_t i = 0; i < bitmap.width; i++){
    for(uint16_t j = 0; j < bitmap.height; j++){
      if(*(bytes + i + j * bitmap.width) != TRANSPARENCY)
        paint_pixel(cursor->x + i, cursor->y + j, *(bytes + i + j * bitmap.width));
    }
  }
}

void (clean_cursor_frame)(){
  xpm_image_t bitmap;
  uint32_t* back_frame;

  if(cursor->isAboveButton || cursor->isAbovePiece)
    bitmap = cursor->select_img;

  else
    bitmap = cursor->img;
  
  back_frame = (uint32_t*)background_frame.bytes;
  for(uint16_t i = 0; i < bitmap.width; i++){
    for(uint16_t j = 0; j < bitmap.height; j++){
      if(*(back_frame + i + j * background_frame.width) != TRANSPARENCY)
        paint_pixel(cursor->x + i, cursor->y + j, *(back_frame + (cursor->x + i) + (cursor->y + j) * background_frame.width));
    }
  }
}

void (update_cursor_position)(struct packet *pack){
  clean_cursor_frame();

  uint16_t oldX = cursor->x;
  uint16_t oldY = cursor->y;

  cursor->x = cursor->x + pack->delta_x;
  cursor->y = cursor->y - pack->delta_y; //Negative, once the Y coordinate increases downwards

  //If the X coordinate goes beyond the left side limit
  if(oldX + pack->delta_x < 0)
    cursor->x = 0;

  //The same thing for the right limit
  else if(cursor->x + cursor->img.width > hres)
    cursor->x = hres - cursor->img.width;

  //If the Y coordinate goes beyond the upper limit
  if(oldY - pack->delta_y < 0)
    cursor->y = 0;

  //The same thing for the lower limit
  else if(cursor->y + cursor->img.height > vres)
    cursor->y = vres - cursor->img.height;

  draw_cursor_frame(); 
}

Collision (cursor_collisions_in_main_menu)(){
  if(cursor->x >= 80 && cursor->x <= 406 && cursor->y >= 339 && cursor->y <= 385){
    cursor->isAboveButton = true;
    return OVER_START_BUTTON;
  }

  else if(cursor->x >= 179 && cursor->x <= 307 && cursor->y >= 440 && cursor->y <= 501){
    cursor->isAboveButton = true;
    return OVER_HELP_BUTTON;
  }

  else if(cursor->x >= 130 && cursor->x <= 356 && cursor->y >= 543 && cursor->y <= 597){
    cursor->isAboveButton = true;
    return OVER_SETTINGS_BUTTON;
  }

  else if(cursor->x >= 179 && cursor->x <= 307 && cursor->y >= 659 && cursor->y <= 710){
    cursor->isAboveButton = true;
    return OVER_EXIT_BUTTON;
  }

  cursor->isAboveButton = false;
  return NO_COLLISION;
}

Collision (cursor_collisions_in_help_menu)(){
  return NO_COLLISION;
}

Collision (cursor_collisions_in_settings_menu)(){
  if(cursor->x >= 55 && cursor->x <= 132 && cursor->y >= 255 && cursor->y <= 332){
    cursor->isAboveButton = true;
    return OVER_PIECE_LEFT_ARROW_BUTTON;
  }

  else if(cursor->x >= 403 && cursor->x <= 480 && cursor->y >= 255 && cursor->y <= 332){
    cursor->isAboveButton = true;
    return OVER_PIECE_RIGHT_ARROW_BUTTON;
  }

  else if(cursor->x >= 55 && cursor->x <= 132 && cursor->y >= 450 && cursor->y <= 527){
    cursor->isAboveButton = true;
    return OVER_CHESSBOARD_LEFT_ARROW_BUTTON;
  }

  else if(cursor->x >= 403 && cursor->x <= 480 && cursor->y >= 450 && cursor->y <= 527){
    cursor->isAboveButton = true;
    return OVER_CHESSBOARD_RIGHT_ARROW_BUTTON;
  }

  cursor->isAboveButton = false;
  return NO_COLLISION;
}

Collision (cursor_collisions_in_pause_menu)(){
  return NO_COLLISION;
}

Piece* (cursor_collided_on_chessboard)(Chessboard* cb){
  for(int i = 0; i < DIM; i++){
    for(int k = 0; k < DIM; k++){
      if(cb->board[i][k]->type != EMPTY){
        uint16_t minX = cb->board[i][k]->x;
        uint16_t maxX = minX + 86;
        uint16_t minY = cb->board[i][k]->y;
        uint16_t maxY = minY + 86;
        

        if(cursor->x > minX && cursor->x < maxX && cursor->y > minY && cursor->y < maxY){
          //draw_digit(200, 300, 0);
          cursor->isAbovePiece = true;
          return cb->board[i][k];
        }
      }
    }
  }

  //draw_digit(200, 300, 10);
  cursor->isAbovePiece = false;
  return NULL;
}
