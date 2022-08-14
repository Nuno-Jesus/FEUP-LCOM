// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

// Any header files included below this line should have been created by you

#include "graphics_card.h"
#include "i8042.h"
#include "kbd.h"

extern uint8_t scancode, stat;
extern vbe_mode_info_t mode_info;
extern unsigned int hres, vres;
extern unsigned int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_graphics_card_mode(mode) != OK){
    vg_exit(); 
    return 1;
  }

  sleep(delay);

  if (vg_exit() != OK) 
    return 1; 

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  uint8_t bit_no = 0;
  int ipc_status,r;
  message msg;
  uint32_t irq_set = BIT(1);

  uint8_t bytes[2];
  int size = 1;

  if (set_graphics_card_mode(mode) != OK){
    vg_exit(); 
    return 1;
  }

  if (kbd_subscribe_int(&bit_no)!= OK) {return 1;}
  if (vg_draw_rectangle(x, y, width, height, color) != OK) {return 1;}

  while(scancode != ESC){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();

            bytes[size - 1] = scancode;

            if(size == 1){
              if(scancode == FIRST_BYTE_OUT_OF_TWO)
                size++;
              
              else
                kbd_print_scancode(!(scancode & BREAKCODE), 1, bytes);
            }

            else{
              kbd_print_scancode(!(scancode & BREAKCODE), 2, bytes);
              size--;
            }
          }
          break;

        default:
          break; 
      }
    }
  }
  
  
  if (kbd_unsubscribe_int()!= OK) return 1;
  if (vg_exit() != OK) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  return 1;
}

int(video_test_controller)() {
  return 1;
}
