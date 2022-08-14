#ifndef PLAYER_H
#define PLAYER_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>

#include "Piece.h"
#include "Digit.h"

#define NUMPIECES 16

typedef struct Player_{
  unsigned int turns;
  unsigned int deadPieces;

  unsigned int time;

  PieceColor color;
  PieceColor oppositeColor;
  
  Piece** set;

} Player;

Player* (new_player)(PieceColor color);

void (delete_player)(Player* p);

void (create_all_players)();

void (delete_all_players)();

void (draw_all_player_pieces)();

void (draw_all_timers)();

void (update_player_timer)(Player* p);

void (draw_player_timer_frame)(Player* p);

void (clean_player_timer_frame)(Player* p);

#endif
