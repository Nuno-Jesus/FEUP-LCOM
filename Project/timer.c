#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"
#include "utils.h"

static int hook_id_timer;
unsigned int counter = 0;

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t x = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  
  if (sys_outb(TIMER_CTRL, x) != 0) 
    return 1;

  if (timer == 0) {
    if (utils_sys_inb(TIMER_0, st) != 0) 
      return 1;
  }

  else if (timer == 1) {
    if (utils_sys_inb(TIMER_1, st) != 0) 
      return 1;
  }

  else if (timer == 2) {
    if (utils_sys_inb(TIMER_2, st) != 0) 
      return 1;
  }

  else{
    return 1;
  }
  
  return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq > TIMER_FREQ || freq < 19)
    return 1;

  uint16_t init = TIMER_FREQ / freq; 
  uint8_t msb = 0, lsb = 0;
  util_get_LSB(init, &lsb);
  util_get_MSB(init, &msb);

  uint8_t conf = 0;
  if (timer_get_conf(timer, &conf) != 0) 
    return 1;
  
  conf = (conf & (TIMER_BCD | TIMER_SQR_WAVE | BIT(3))) | TIMER_LSB_MSB;

  if(sys_outb(TIMER_CTRL, conf) != 0) return 1;
  if(sys_outb(TIMER_0, lsb) != 0) return 1;
  if(sys_outb(TIMER_0, msb) != 0) return 1;
  
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id_timer = 0;
  *bit_no = hook_id_timer;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != 0) 
    return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_timer) != 0) {return 1;}

  return 0;
}

void (timer_int_handler)() {
  counter++;
}


int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field){
  union timer_status_field_val tsf;

  if (field == tsf_all)
    tsf.byte = st;
  
  //status = 00xx 0000
  else if (field == tsf_initial){
    uint8_t status = st & (TIMER_LSB | TIMER_MSB);

    switch(status){
      case 0x00:
        tsf.in_mode = INVAL_val;
        break;

      case 0x10:
        tsf.in_mode = LSB_only;
        break;

      case 0x20:
        tsf.in_mode = MSB_only;
        break;

      case 0x30:
        tsf.in_mode = MSB_after_LSB;
        break;

    }
  }

  //status = 0000 xxx0
  else if (field == tsf_mode){
    uint8_t status = st & (TIMER_SQR_WAVE | BIT(3));

    switch (status){
      case 0x00:
        tsf.count_mode = 0; 
        break;

      case 0x02:
        tsf.count_mode = 1; 
        break;

      case 0x04:
        tsf.count_mode = 2; 
        break;

      case 0x06:
        tsf.count_mode = 3; 
        break;

      case 0x08:
        tsf.count_mode = 4; 
        break;

      case 0x0A:
        tsf.count_mode = 5; 
        break;

      case 0x0C:
        tsf.count_mode = 2; 
        break;

      case 0x0E:
        tsf.count_mode = 3; 
        break;

      default:
        break;
    }
  }

  else if (field == tsf_base){
    if ((st & TIMER_BCD) == 0)
      tsf.bcd = false;
    else
      tsf.bcd = true;
  }

  if (timer_print_config(timer, field, tsf) != 0) 
    return 1;

  return 0;
}
