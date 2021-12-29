#ifndef UTILS_H
#define UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>

int (utils_sys_inb)(int port, uint8_t *value);

int (util_get_LSB)(uint16_t val, uint8_t *lsb);

int (util_get_MSB)(uint16_t val, uint8_t *msb);

uint16_t (convert_2_complement)(uint8_t number);

void (swap1)(uint16_t *a, uint16_t *b);

void (swap2)(int *a, int *b);


#endif
