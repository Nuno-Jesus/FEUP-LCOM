#ifndef PIECE_H
#define PIECE_H

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "graphics_card.h"
#include "utils.h"

#include "SettingsStyle.h"

#include "images/pieces/white_bishop.xpm"
#include "images/pieces/white_pawn.xpm"
#include "images/pieces/white_rook.xpm"
#include "images/pieces/white_knight.xpm"
#include "images/pieces/white_queen.xpm"
#include "images/pieces/white_king.xpm"

#include "images/pieces/black_bishop.xpm"
#include "images/pieces/black_pawn.xpm"
#include "images/pieces/black_rook.xpm"
#include "images/pieces/black_knight.xpm"
#include "images/pieces/black_queen.xpm"
#include "images/pieces/black_king.xpm"

#include "images/pieces/empty.xpm"

#define FIRST_X 41
#define FIRST_Y 39

#define BLUE 0x0061F0
#define RED 0xFD3D3D

typedef enum PieceColor_{
  NO_COLOR,
  BLACK,
  WHITE
} PieceColor;

typedef enum PieceType_{
  EMPTY,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
  QUEEN,
  KING
} PieceType;

typedef struct Piece_{
  PieceColor color;
  PieceType type;

  unsigned int ID;
  
  //Piece xpm coordinates
  uint16_t x;
  uint16_t y;

  //Piece coordinates on the board 
  int posX;
  int posY;

  bool isDead;
  bool wasPromoted;
  bool canBePromoted;
  
  xpm_image_t img;
} Piece;

Piece* (new_piece)(PieceColor color, PieceType type, int x, int y, int posX, int posY, unsigned int ID);

void (delete_piece)(Piece* p);

void (draw_piece)(Piece* p);

void (clean_piece)(Piece* p);

void (move_piece)(Piece* p, Piece* p2);

void (load_piece_xpm)(Piece *p);

void (load_piece_standard_xpm)(Piece *p);

void (load_piece_midnight_xpm)(Piece *p);

void (insert_piece_into_back_frame)(Piece* p);

void (toggle_piece_blue_highlight)(Piece *p);

void (untoggle_piece_blue_highlight)(Piece* p);

void (toggle_piece_red_highlight)(Piece* p);

void (untoggle_piece_red_highlight)(Piece* p);

#endif
