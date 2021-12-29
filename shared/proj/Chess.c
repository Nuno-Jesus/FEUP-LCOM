#include "Chess.h"

void (get_valid_moves)(Piece* p, bool validMoves[][8], Piece* board[][8]){
  switch(p->type){
    case PAWN:
      pawn(p, validMoves, board);
      break;
      
    case KNIGHT:
      knight(p, validMoves, board);
      break;

    case BISHOP:
      bishop(p, validMoves, board);
      break;

    case ROOK:
      rook(p, validMoves, board);
      break;

    case QUEEN:
      queen(p, validMoves, board);
      break;

    case KING:
      king(p, validMoves, board);
      break;

    default:
      break;
  }
}

bool (validate_move)(bool validMoves[][8], int *finalX, int *finalY){
  uint16_t x;
  uint16_t y;

  retrieve_cursor_coordinates(&x, &y);

  x = (x - FIRST_X)/86;
  y = (y - FIRST_Y)/86;

  *finalX = x;
  *finalY = y;

  return validMoves[y][x];
}

void (display_valid_moves)(bool validMoves[][8], Piece* board[][8]){
  for(int i = 0; i < DIM; i++){
    for(int k = 0; k < DIM; k++){
      if(validMoves[i][k] == true){
        toggle_piece_red_highlight(board[i][k]);
        draw_piece(board[i][k]);
      }
    }
  }
}

void (clean_valid_moves)(bool validMoves[][8], Piece* board[][8]){
  for(int i = 0; i < DIM; i++){
    for(int k = 0; k < DIM; k++){
      if(validMoves[i][k] == true){
        untoggle_piece_red_highlight(board[i][k]);
        remove_piece_from_back_frame(board[i][k]->img, board[i][k]->x, board[i][k]->y);
        clean_piece(board[i][k]);
        draw_piece(board[i][k]);
      }
    }
  }
}

void (reset_valid_moves)(bool validMoves[][8]){
  for(int i = 0; i < DIM; i++){
    for(int k = 0; k < DIM; k++){
      validMoves[i][k] = false;
    }
  }
}

void (pawn)(Piece* p, bool validMoves[][8], Piece* board[][8]){
  if(p->color == WHITE){
    //If the white pawn hasn't moved yet
    if(p->posY == 6){
      //If the square above is free
      if(board[5][p->posX]->type == EMPTY){
        validMoves[5][p->posX] = true;

        //Also if the next square above is free
        if(board[4][p->posX]->type == EMPTY){
          validMoves[4][p->posX] = true;
        }
      }

      //If a black piece is diagonally to its left
      if(p->posX - 1 >= 0){
        if(board[5][p->posX - 1]->color == BLACK)
          validMoves[5][p->posX - 1] = true;
      }

      //If a black piece is diagonally to its right
      if(p->posX + 1 < DIM){
        if(board[5][p->posX + 1]->color == BLACK)
          validMoves[5][p->posX + 1] = true;  
      }
    }

    //If the white pawn has moved before
    else if(p->posY > 0 && p->posY < DIM - 1){
      //If the square above is free
      if(board[p->posY - 1][p->posX]->type == EMPTY)
        validMoves[p->posY - 1][p->posX] = true;
      
      //If the left diagonal square has a black piece
      if(p->posX - 1 >= 0){
        if(board[p->posY - 1][p->posX - 1]->color == BLACK)
          validMoves[p->posY - 1][p->posX - 1] = true;
      }

      //Same for the right diagonal
      if(p->posX < DIM){
        if(board[p->posY - 1][p->posX + 1]->color == BLACK)
          validMoves[p->posY - 1][p->posX + 1] = true;
      }
    }
  }
  
  else{
    //If the black pawn hasn't moved yet
    if(p->posY == 1){
      //If the square below is free
      if(board[2][p->posX]->type == EMPTY){
        validMoves[2][p->posX] = true;

        //Also if the next square below is free
        if(board[3][p->posX]->type == EMPTY){
          validMoves[3][p->posX] = true;
        }
      }

      //If a white piece is diagonally to its left
      if(p->posX + 1 >= 0){
        if(board[2][p->posX + 1]->color == WHITE)
          validMoves[2][p->posX + 1] = true;
      }

      //If a white piece is diagonally to its right
      if(p->posX + 1 < DIM){
        if(board[2][p->posX + 1]->color == WHITE)
          validMoves[2][p->posX + 1] = true;  
      }
    }

    //If the black pawn has moved before
    else if(p->posY > 0 && p->posY < DIM - 1){
      //If the square above is free
      if(board[p->posY - 1][p->posX]->type == EMPTY)
        validMoves[p->posY - 1][p->posX] = true;
      

      //If the left diagonal square has a white piece
      if(p->posX - 1 >= 0){
        if(board[p->posY - 1][p->posX - 1]->color == WHITE)
          validMoves[p->posY - 1][p->posX - 1] = true;
      }

      //Same for the right diagonal
      if(p->posX + 1 < DIM){
        if(board[p->posY - 1][p->posX + 1]->color == WHITE)
          validMoves[p->posY - 1][p->posX + 1] = true;
      }
    }
  }
}

void (knight)(Piece* p, bool validMoves[][8], Piece* board[][8]){
  //2 Left 1 Up
  if(p->posY - 1 > 0 && p->posX - 2 >= 0){
    if(board[p->posY - 1][p->posX - 2]->color != p->color)
      validMoves[p->posY - 1][p->posX - 2] = true;    
  }

  //1 Left 2 Up
  if(p->posY - 2 > 0 && p->posX - 1 >= 0){
    if(board[p->posY - 2][p->posX - 1]->color != p->color)
      validMoves[p->posY - 2][p->posX - 1] = true;
  }

  //1 Right 2 Up
  if(p->posY - 2 >= 0 && p->posX + 1 < DIM){
    if(board[p->posY - 2][p->posX + 1]->color != p->color)
      validMoves[p->posY - 2][p->posX + 1] = true;
  }

  //2 Right 1 Up
  if(p->posY - 1 >= 0 && p->posX + 2 < DIM){
    if(board[p->posY - 1][p->posX + 2]->color != p->color)
      validMoves[p->posY - 1][p->posX + 2] = true;
  }

  //2 Right 1 Down
  if(p->posY + 1 < DIM && p->posX + 2 < DIM){
    if(board[p->posY + 1][p->posX + 2]->color != p->color)
      validMoves[p->posY + 1][p->posX + 2] = true;
  }

  //1 Right 2 Down
  if(p->posY + 2 < DIM && p->posX + 1 < DIM){
    if(board[p->posY + 2][p->posX + 1]->color != p->color)
      validMoves[p->posY + 2][p->posX + 1] = true;
  }

  //1 Left 2 Down
  if(p->posY + 2 < DIM && p->posX - 1 >= 0){
    if(board[p->posY + 2][p->posX - 1]->color != p->color)
      validMoves[p->posY + 2][p->posX - 1] = true;
  }

  //2 Left 1 Down
  if(p->posY + 1 < DIM && p->posX - 2 >= 0){
    if(board[p->posY + 1][p->posX - 2]->color != p->color)
      validMoves[p->posY + 1][p->posX - 2] = true;
  }
}

void (bishop)(Piece* p, bool validMoves[][8], Piece* board[][8]){
  //Left upper diagonal
  for(int x = 0, y = 0; p->posX + x > 0 && p->posY + y > 0; x--, y--){
    if(board[p->posY + y][p->posX + x]->color == p->color)
      break;

    else 
      validMoves[p->posY + y][p->posX + x] = true;
  }

  //Left lower diagonal
  for(int x = 0, y = 0; p->posX + x > 0 && p->posY + y < DIM; x--, y++){
    if(board[p->posY + y][p->posX + x]->color == p->color)
      break;

    else 
      validMoves[p->posY + y][p->posX + x] = true;
  }

  //Right lower diagonal
  for(int x = 0, y = 0; p->posX + x < DIM && p->posY + y < DIM; x++, y++){
    if(board[p->posY + y][p->posX + x]->color == p->color)
      break;

    else 
      validMoves[p->posY + y][p->posX + x] = true;
  }

  //Right upper diagonal
  for(int x = 0, y = 0; p->posX + x < DIM && p->posY + y > 0; x++, y--){
    if(board[p->posY + y][p->posX + x]->color == p->color)
      break;

    else 
      validMoves[p->posY + y][p->posX + x] = true;
  }
}

void (rook)(Piece* p, bool validMoves[][8], Piece* board[][8]){

}

void (queen)(Piece* p, bool validMoves[][8], Piece* board[][8]){

}

void (king)(Piece* p, bool validMoves[][8], Piece* board[][8]){

}
