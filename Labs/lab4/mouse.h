#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "utils4.h"

bool (buffer_is_full)(uint8_t buffer);

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (mouse_assemble_packet)(struct packet * pacote);

int (mouse_enable_data_report)();

int (mouse_disable_data_report)();

int (mouse_write_command)(uint32_t command, uint8_t *response);

struct mouse_ev* (mouse_events)(struct packet *pack);
