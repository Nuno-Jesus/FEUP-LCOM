#include <lcom/lcf.h>

#include "i8254.h"
#include <stdint.h>

int (util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (val & 0x00FF);

  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (val & 0xFF00) >> 8;

  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  if (sys_inb(port, &val) != 0)
    return 1;
  
  *value = (val & 0x000000FF);

  return 0;
}
