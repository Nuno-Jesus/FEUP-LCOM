#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8042.h"
#include "utils.h"
#include "kbd.h"

extern uint8_t scancode, status;
extern int count_in;
extern unsigned int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(kbd_test_scan)() {

  uint8_t bit_no = 0;
  int ipc_status,r;
  message msg;
  uint32_t irq_set = BIT(1);
  
  uint8_t bytes[2];
  int size = 1;


  if(kbd_subscribe_int(&bit_no) != OK) 
    return 1;

  while(scancode != ESC){
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK){ 
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
  
  if (kbd_unsubscribe_int() != OK) 
    return 1;

  return 0;
}

int(kbd_test_poll)() {
  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  return 1;
}

