#include "utils4.h"


int (utils_sys_inb)(int port, uint8_t *value) {
  uint32_t val32;
  if (sys_inb(port, &val32) != 0) {return 4;}
    *value = (val32 & 0x000000FF);

  return 0;
}

uint16_t (convert_2_complement)(uint8_t number){
  return number - 256;
}



