// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"

extern uint8_t scancode;
extern int hook_id;
extern int counter;


uint8_t packet_bytes[3];
struct packet pack;


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
  uint8_t bit_no = 0;
  int ipc_status, r;
  message msg;
  uint32_t irq_set = BIT(MOUSE_IRQ);

  int size = 1;

  if (mouse_enable_data_report() != OK) return 1;
  if (mouse_subscribe_int(&bit_no) != OK) return 1;

  while(cnt > 0){
      if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) { 
        printf("driver_receive failed with: %d", r);
        continue;
      }

      if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE: 
            if (msg.m_notify.interrupts & irq_set) {
              mouse_ih();
              
              packet_bytes[size - 1] = scancode;

              if(size == 1 && (scancode & FIRST_BYTE)){
                size++;
              }

              else if(size == 2){
                size++;
              }

              else if(size == 3){
                for(int i = 0; i < 3; i++)
                  pack.bytes[i] = packet_bytes[i];

                mouse_assemble_packet(&pack);
                mouse_print_packet(&pack);

                size = 1;
                cnt--; 
              }
            }

            break;

          default:
            break; 
        }
      }
  }

  if (mouse_unsubscribe_int() != OK) return 1;
  if (mouse_disable_data_report() != OK) return 1;
  
  
  return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  return 0;
}

int (mouse_test_gesture)(uint8_t xlen, uint8_t tolerance) {
  return 0;
}
