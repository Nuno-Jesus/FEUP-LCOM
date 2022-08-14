#include "Digit.h"

xpm_image_t digits[10];

//From "Menu.c"
extern xpm_image_t background_frame;
extern Menu** all_menus;

void (load_all_digits)(){
  for(int i = 0; i <= 10; i++)
    load_digit(i);
}

void (load_digit)(int number){
  switch(number){
    case 0: xpm_load(zero_xpm, XPM_8_8_8_8, &(digits[0])); break;
    case 1: xpm_load(one_xpm, XPM_8_8_8_8, &(digits[1])); break;
    case 2: xpm_load(two_xpm, XPM_8_8_8_8, &(digits[2])); break;
    case 3: xpm_load(three_xpm, XPM_8_8_8_8, &(digits[3])); break;
    case 4: xpm_load(four_xpm, XPM_8_8_8_8, &(digits[4])); break;
    case 5: xpm_load(five_xpm, XPM_8_8_8_8, &(digits[5])); break;
    case 6: xpm_load(six_xpm, XPM_8_8_8_8, &(digits[6])); break;
    case 7: xpm_load(seven_xpm, XPM_8_8_8_8, &(digits[7])); break;
    case 8: xpm_load(eight_xpm, XPM_8_8_8_8, &(digits[8])); break;
    case 9: xpm_load(nine_xpm, XPM_8_8_8_8, &(digits[9])); break;
    case 10: xpm_load(dots_xpm, XPM_8_8_8_8, &(digits[10])); break;
    default: break;
  }
}

void (draw_digit)(uint16_t x, uint16_t y, unsigned int number){
  uint32_t* bytes = (uint32_t*)digits[number].bytes;
  
  for(uint16_t i = 0; i < digits[number].width; i++){
    for(uint16_t j = 0; j < digits[number].height; j++){
      if(*(bytes + i + j*digits[number].width) != TRANSPARENCY)
        paint_pixel(x + i, y + j, *(bytes + i + j*digits[number].width));
    }
  }

  insert_digit_into_back_frame(x, y, number);
}

void (clean_digit)(uint16_t x, uint16_t y, unsigned int number){
  uint32_t* back_frame = (uint32_t*)background_frame.bytes;
  
  for(uint16_t i = 0; i < digits[number].width; i++){
    for(uint16_t j = 0; j < digits[number].height; j++){
      paint_pixel(x + i, y + j, *(back_frame + i + j*background_frame.width));
    }
  }

  remove_digit_from_back_frame(x, y, number);
}

void (insert_digit_into_back_frame)(uint16_t x, uint16_t y, unsigned int number){
  uint32_t* bytes = (uint32_t*)digits[number].bytes;
  uint32_t* back_frame = (uint32_t*)background_frame.bytes;

  for(uint16_t i = 0; i < digits[number].width; i++){
    for(uint16_t j = 0; j < digits[number].height; j++){
      if(*(bytes + i + j*digits[number].width) != TRANSPARENCY)
        *(back_frame + (x + i) + (y + j)*background_frame.width) = *(bytes + i + j*digits[number].width);
    }
  }
}

void (remove_digit_from_back_frame)(uint16_t x, uint16_t y, unsigned int number){
  uint32_t* menu_bytes = (uint32_t*)all_menus[PLAY_MENU]->img.bytes;
  uint32_t* back_frame = (uint32_t*)background_frame.bytes;

  for(uint16_t i = 0; i < digits[number].width; i++){
    for(uint16_t j = 0; j < digits[number].height; j++){
      *(back_frame + (x + i) + (y + j)*background_frame.width) = *(menu_bytes + (x + i) + (y + j)*all_menus[PLAY_MENU]->img.width);
    }
  }
}
