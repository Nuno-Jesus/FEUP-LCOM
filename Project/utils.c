#include "utils.h"

int (utils_sys_inb)(int port, uint8_t *value){
  uint32_t val;
  if (sys_inb(port, &val) != OK)
    return 1;

  *value = (val & 0x000000FF);

  return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb){
  *lsb = (val & 0x00FF);

  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb){
  *msb = (val & 0xFF00) >> 8;

  return 0;
}

uint16_t (convert_2_complement)(uint8_t number){
  return number - 256;
}

void (swap1)(uint16_t *a, uint16_t *b){
  uint16_t aux = *a;
  *a = *b;
  *b = aux;

}
void (swap2)(int *a, int *b){
  int aux = *a;
  *a = *b;
  *b = aux;
}

