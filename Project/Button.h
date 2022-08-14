#ifndef BUTTON_H
#define BUTTON_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "graphics_card.h"

#include "images/buttons/start_button.xpm"
#include "images/buttons/start_over_button.xpm"
#include "images/buttons/start_pressed_button.xpm"

#include "images/buttons/options_button.xpm"
#include "images/buttons/options_over_button.xpm"
#include "images/buttons/options_pressed_button.xpm"

#include "images/buttons/help_button.xpm"
#include "images/buttons/help_over_button.xpm"
#include "images/buttons/help_pressed_button.xpm"

#include "images/buttons/exit_button.xpm"
#include "images/buttons/exit_over_button.xpm"
#include "images/buttons/exit_pressed_button.xpm"

#include "images/buttons/right_button.xpm"
#include "images/buttons/right_over_button.xpm"
#include "images/buttons/right_pressed_button.xpm"

#include "images/buttons/left_button.xpm"
#include "images/buttons/left_over_button.xpm"
#include "images/buttons/left_pressed_button.xpm"

typedef enum ButtonType_{
  START_BUTTON,
  HELP_BUTTON,
  SETTINGS_BUTTON,
  EXIT_BUTTON,
  CONTINUE_BUTTON,
  GO_BACK_BUTTON,
  PIECE_LEFT_ARROW_BUTTON,
  PIECE_RIGHT_ARROW_BUTTON,
  CHESSBOARD_LEFT_ARROW_BUTTON,
  CHESSBOARD_RIGHT_ARROW_BUTTON
} ButtonType;

typedef struct Button_{
  uint16_t x, y;

  ButtonType type;

  bool mouseIsOver;
  bool hasBeenPressed;

  xpm_image_t img;
  xpm_image_t over_img;
  xpm_image_t pressed_img;

} Button;

Button* (new_button)(uint16_t x, uint16_t y, ButtonType type);

void (delete_button)(Button* b);

void (draw_button)(Button* b);

void (clean_button)(Button* b);

void (insert_button_into_back_frame)(Button* b);

#endif
