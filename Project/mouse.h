#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "utils.h"
#include "graphics_card.h"

#include "Menu.h"

#include "images/cursor/cursor.xpm"
#include "images/cursor/select_cursor.xpm"

typedef enum Collision_{
  NO_COLLISION,
  OVER_START_BUTTON,
  OVER_HELP_BUTTON,
  OVER_SETTINGS_BUTTON,
  OVER_EXIT_BUTTON,
  OVER_CONTINUE_BUTTON,
  OVER_GO_BACK_BUTTON,
  OVER_PIECE_RIGHT_ARROW_BUTTON,
  OVER_PIECE_LEFT_ARROW_BUTTON,
  OVER_CHESSBOARD_RIGHT_ARROW_BUTTON,
  OVER_CHESSBOARD_LEFT_ARROW_BUTTON,
} Collision;

typedef struct Cursor_{
  uint16_t x, y;

  bool isAboveButton;
  bool isAbovePiece;

  xpm_image_t img;
  xpm_image_t select_img;
} Cursor;


bool (buffer_is_full)(uint8_t buffer);

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

void (mouse_assemble_packet)(struct packet * pacote);

int (mouse_enable_data_report)();

int (mouse_disable_data_report)();

int (mouse_write_command)(uint32_t command, uint8_t *response);

struct mouse_ev (mouse_events)(struct packet *pack);

/****************************** CURSOR ******************************/

void (new_cursor)(uint16_t x, uint16_t y);

void (retrieve_cursor_coordinates)(uint16_t* x, uint16_t* y);

void (draw_cursor_frame)();

void (clean_cursor_frame)();

void (update_cursor_position)(struct packet *pack);

Collision (cursor_collisions_in_main_menu)();

Collision (cursor_collisions_in_help_menu)();

Collision (cursor_collisions_in_settings_menu)();

Collision (cursor_collisions_in_pause_menu)();

Piece* (cursor_collided_on_chessboard)(Chessboard* cb);

#endif
