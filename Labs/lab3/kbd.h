#ifndef KBD_H
#define KBD_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "utils.h"


int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

void (kbc_ih)();

int (kbd_enable)();

#endif
