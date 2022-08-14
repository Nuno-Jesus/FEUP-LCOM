#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id;
unsigned int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /*
    Como queremos um número de 16 bits, se freq for menor que 19, TIMER_FREQ / freq vai dar um número que é maior que o máximo representável em 16 bits (2^16 = 65536), logo, freq deve ser maior que 19, e menor que o valor de TIMER_FREQ.
  */
  if (freq > TIMER_FREQ || freq < 19)
    return 1;

  uint16_t init = TIMER_FREQ / freq; //Obter LSB+MSB novo
  uint8_t initmsb = 0, initlsb = 0;
  util_get_LSB(init, &initlsb);
  util_get_MSB(init, &initmsb);

  uint8_t conf = 0;
  if (timer_get_conf(timer, &conf) != 0) {return 1;}

  //Guardar os 4 LSB e colocar no modo LSB followed by MSB
  conf = (conf & (TIMER_BCD | TIMER_SQR_WAVE | BIT(3))) | TIMER_LSB_MSB;
  
  if (timer == 0) {
    conf = conf | TIMER_SEL0;

    if(sys_outb(TIMER_CTRL, conf) != 0) {return 2;}
    if(sys_outb(TIMER_0, initlsb) != 0) {return 2;}
    if(sys_outb(TIMER_0, initmsb) != 0) {return 2;}

    return 0;
  }
  else if (timer == 1) {
    conf = conf | TIMER_SEL1;

    if(sys_outb(TIMER_CTRL, conf) != 0) {return 2;}
    if(sys_outb(TIMER_1, initlsb) != 0) {return 2;}
    if(sys_outb(TIMER_1, initmsb) != 0) {return 2;}

    return 0;
  }
  else if (timer == 2) {
    conf = conf | TIMER_SEL2;

    if(sys_outb(TIMER_CTRL, conf) != 0) {return 2;}
    if(sys_outb(TIMER_2, initlsb) != 0) {return 2;}
    if(sys_outb(TIMER_2, initmsb) != 0) {return 2;}

    return 0;
  }
  
  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = 0;
  *bit_no = hook_id;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0) {return 1;}

  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) {return 1;}

  return 0;
}

void (timer_int_handler)() {
  counter++;
}


int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val tsf_union;

  if (field == tsf_all)
  {
    tsf_union.byte = st;
  }

  else if (field == tsf_initial)
  {
    tsf_union.in_mode = st;

    if ((st & TIMER_LSB) == 0)
      if ((st & TIMER_MSB) == 0)
        tsf_union.count_mode = INVAL_val;
      else
        tsf_union.count_mode = MSB_only;
    else
      if ((st & TIMER_MSB) == 0)
        tsf_union.count_mode = LSB_only;
      else
        tsf_union.count_mode = MSB_after_LSB;
  }

  else if (field == tsf_mode)
  {
    tsf_union.count_mode = st;

    int x = st & (TIMER_SQR_WAVE | BIT(3));

    switch (x)
    {
      case 0:
        tsf_union.count_mode = 0; break;
      case 2:
        tsf_union.count_mode = 1; break;
      case 4:
        tsf_union.count_mode = 2; break;
      case 6:
        tsf_union.count_mode = 3; break;
      case 8:
        tsf_union.count_mode = 4; break;
      case 10:
        tsf_union.count_mode = 5; break;
      case 12:
        tsf_union.count_mode = 2; break;
      case 14:
        tsf_union.count_mode = 3; break;
      default:
        return 1;
        break;
    }
  }

  else if (field == tsf_base)
  {
    tsf_union.bcd = st;

    if ((st & TIMER_BCD) == 0)
      tsf_union.bcd = false;
    else
      tsf_union.bcd = true;
  }

  if (timer_print_config(timer, field, tsf_union) != 0) {return 1;}

  return 0;
}
