#include "utils.h"

int (utils_sys_inb)(int port, uint8_t *value) {
  uint32_t val;
  if (sys_inb(port, &val) != OK)
    return 1;

  *value = (val & 0x000000FF);

  return 0;
}

