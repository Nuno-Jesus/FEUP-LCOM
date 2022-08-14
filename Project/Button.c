#include "Button.h"

//From "graphics_card.c"
extern unsigned int hres;
extern unsigned int vres;

//From "Menu.c"
extern xpm_image_t background_frame;

Button* (new_button)(uint16_t x, uint16_t y, ButtonType type){
  Button* b = (Button*) malloc(sizeof(Button));
  if(b == NULL)
    return NULL;

  b->x = x;
  b->y = y;
  b->type = type;

  b->mouseIsOver = false;
  b->hasBeenPressed = false;

  switch(type){
    case START_BUTTON:
      xpm_load(start_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(start_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(start_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;

    case HELP_BUTTON:
      xpm_load(help_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(help_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(help_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;

    case SETTINGS_BUTTON:
      xpm_load(options_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(options_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(options_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;

    case EXIT_BUTTON:
      xpm_load(exit_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(exit_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(exit_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;

    case CONTINUE_BUTTON:
      break;

    case GO_BACK_BUTTON:
      break;

    case PIECE_RIGHT_ARROW_BUTTON:
      xpm_load(right_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(right_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(right_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;

    case PIECE_LEFT_ARROW_BUTTON:
      xpm_load(left_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(left_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(left_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;
    
    case CHESSBOARD_RIGHT_ARROW_BUTTON:
      xpm_load(right_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(right_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(right_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;

    case CHESSBOARD_LEFT_ARROW_BUTTON:
      xpm_load(left_button_xpm, XPM_8_8_8_8, &(b->img));
      xpm_load(left_over_button_xpm, XPM_8_8_8_8, &(b->over_img));
      xpm_load(left_pressed_button_xpm, XPM_8_8_8_8, &(b->pressed_img));
      break;

    default:
      break;
  }

  return b;
}

void (delete_button)(Button* b){
  free(b);
}

void (draw_button)(Button* b){
  xpm_image_t bitmap;
  uint32_t *button_bytes;

  if(b->mouseIsOver){
    bitmap = b->over_img;
  }

  else if(b->hasBeenPressed){
    bitmap = b->pressed_img;
  }

  else{
    bitmap = b->img;
  }

  button_bytes = (uint32_t*) bitmap.bytes;
  for(uint16_t i = 0; i < bitmap.width; i++){
    for(uint16_t j = 0; j < bitmap.height; j++){
      if(*(button_bytes + i + j * bitmap.width) != TRANSPARENCY)
        paint_pixel(b->x + i, b->y + j, *(button_bytes + i + j * bitmap.width));
    }
  }
}

void (clean_button)(Button* b){
  xpm_image_t bitmap;
  uint32_t *back_frame;

  if(b->mouseIsOver){
    bitmap = b->over_img;
  }

  else if(b->hasBeenPressed){
    bitmap = b->pressed_img;
  }

  else{
    bitmap = b->img;
  }

  back_frame = (uint32_t*)background_frame.bytes;

  for(uint16_t i = 0; i < bitmap.width; i++){
    for(uint16_t j = 0; j < bitmap.height; j++){
      if(*(back_frame + (b->x + i) + (b->y + j) * background_frame.width) != TRANSPARENCY)
        paint_pixel(b->x + i, b->y + j, *(back_frame + (b->x + i) + (b->y + j) * background_frame.width));
    }
  }
}

void (insert_button_into_back_frame)(Button* b){
  xpm_image_t bitmap;

  uint32_t *button_bytes;
  uint32_t *back_frame_bytes;

  if(b->mouseIsOver){
    bitmap = b->over_img;
  }

  else if(b->hasBeenPressed){
    bitmap = b->pressed_img;
  }

  else{
    bitmap = b->img;
  }

  button_bytes = (uint32_t*)bitmap.bytes;
  back_frame_bytes = (uint32_t*)background_frame.bytes;

  for(uint16_t i = 0; i < bitmap.width; i++){
    for(uint16_t j = 0; j < bitmap.height; j++){
      if(*(button_bytes + i + j * bitmap.width) != TRANSPARENCY)
        *(back_frame_bytes + (b->x + i) + (b->y + j) * background_frame.width) = *(button_bytes + i + j * bitmap.width);
    }
  }
}
