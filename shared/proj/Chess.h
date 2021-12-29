#ifndef CHESS_H
#define CHESS_H

#include <lcom/lcf.h>

#include "mouse.h"

#include "Piece.h"

void (get_valid_moves)(Piece* p, bool validMoves[][8], Piece* board[][8]);

void (display_valid_moves)(bool validMoves[][8], Piece* board[][8]);

void (clean_valid_moves)(bool validMoves[][8], Piece* board[][8]);

void (reset_valid_moves)(bool validMoves[][8]);

void (pawn)(Piece* p, bool validMoves[][8], Piece* board[][8]);

void (knight)(Piece* p, bool validMoves[][8], Piece* board[][8]);

void (bishop)(Piece* p, bool validMoves[][8], Piece* board[][8]);

void (rook)(Piece* p, bool validMoves[][8], Piece* board[][8]);

void (queen)(Piece* p, bool validMoves[][8], Piece* board[][8]);

void (king)(Piece* p, bool validMoves[][8], Piece* board[][8]);

bool (validate_move)(bool validMoves[][8], int *finalX, int *finalY);

#endif
