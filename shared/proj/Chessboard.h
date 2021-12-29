#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "graphics_card.h"

#include "Player.h"
#include "Piece.h"
#include "SettingsStyle.h"

#include "images/boards/chessboard.xpm"
#include "images/boards/glass_chessboard.xpm"
#include "images/boards/cherry_chessboard.xpm"
#include "images/boards/grey_chessboard.xpm"
#include "images/boards/mint_chessboard.xpm"

#define DIM 8

typedef struct Chessboard_{
  Piece* board[DIM][DIM];
  
  uint16_t x;
  uint16_t y;

  xpm_image_t img;
} Chessboard;

Chessboard* (new_chessboard)(uint16_t x, uint16_t y);

void (delete_chessboard)(Chessboard* cb);

void (empty_chessboard)(Chessboard* cb);

void (load_board_xpm)(Chessboard* cb);

void (fill_chessboard)(Chessboard* cb);

void (draw_chessboard)(Chessboard* cb);

void (insert_chessboard_into_back_frame)(Chessboard* cb);

#endif

