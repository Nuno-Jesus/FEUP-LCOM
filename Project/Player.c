#include "Player.h"

#define MAXTIME 300

Player* blackPlayer;
Player* whitePlayer;

Player* (new_player)(PieceColor color){
  Player* p = (Player*)malloc(sizeof(Player));
  if(p == NULL)
    return NULL;

  p->turns = 0;
  p->deadPieces = 0;
  p->time = MAXTIME;
  p->color = color;
  p->oppositeColor = !color;
  p->set = (Piece**) malloc(16 * sizeof(Piece*));
  if(p->set == NULL){
    free(p);
    return NULL;
  }

  if(color == WHITE){
    for(int i = 0; i < 8; i++){
      p->set[i] = new_piece(color, PAWN, FIRST_X + i*86, FIRST_Y + 6*86, i, 6, i);
    }

    p->set[8] = new_piece(color, ROOK,    FIRST_X + 0*86, FIRST_Y + 7*86, 0, 7, 8);
    p->set[9] = new_piece(color, KNIGHT,  FIRST_X + 1*86, FIRST_Y + 7*86, 1, 7, 9);
    p->set[10] = new_piece(color, BISHOP, FIRST_X + 2*86, FIRST_Y + 7*86, 2, 7, 10);
    p->set[11] = new_piece(color, KING,   FIRST_X + 3*86, FIRST_Y + 7*86, 3, 7, 11);
    p->set[12] = new_piece(color, QUEEN,  FIRST_X + 4*86, FIRST_Y + 7*86, 4, 7, 12);
    p->set[13] = new_piece(color, BISHOP, FIRST_X + 5*86, FIRST_Y + 7*86, 5, 7, 13);
    p->set[14] = new_piece(color, KNIGHT, FIRST_X + 6*86, FIRST_Y + 7*86, 6, 7, 14);
    p->set[15] = new_piece(color, ROOK,   FIRST_X + 7*86, FIRST_Y + 7*86, 7, 7, 15);

  }

  else if(color == BLACK){
    for(int i = 0; i < 8; i++){
      p->set[i] = new_piece(color, PAWN, FIRST_X + i*86, 39 + 1*86, i, 1, i);
    }

    p->set[8] = new_piece(color, ROOK,    FIRST_X + 0*86, FIRST_Y + 0*86, 0, 0, 8);
    p->set[9] = new_piece(color, KNIGHT,  FIRST_X + 1*86, FIRST_Y + 0*86, 1, 0, 9);
    p->set[10] = new_piece(color, BISHOP, FIRST_X + 2*86, FIRST_Y + 0*86, 2, 0, 10);
    p->set[11] = new_piece(color, KING,   FIRST_X + 3*86, FIRST_Y + 0*86, 3, 0, 11);
    p->set[12] = new_piece(color, QUEEN,  FIRST_X + 4*86, FIRST_Y + 0*86, 4, 0, 12);
    p->set[13] = new_piece(color, BISHOP, FIRST_X + 5*86, FIRST_Y + 0*86, 5, 0, 13);
    p->set[14] = new_piece(color, KNIGHT, FIRST_X + 6*86, FIRST_Y + 0*86, 6, 0, 14);
    p->set[15] = new_piece(color, ROOK,   FIRST_X + 7*86, FIRST_Y + 0*86, 7, 0, 15);

  }

  else{
    free(p->set);
    free(p);
    return NULL;
  }

  return p;
}

void (create_all_players)(){
  blackPlayer = new_player(BLACK);
  whitePlayer = new_player(WHITE);
}

void (delete_player)(Player* p){
  for(int i = 0; i < 16; i++)
    delete_piece(p->set[i]);

  free(p->set);
  free(p);
}

void (delete_all_players)(){
  delete_player(blackPlayer);
  delete_player(whitePlayer);
}

void (draw_all_player_pieces)(){
  for(int i = 0; i < NUMPIECES; i++){
    draw_piece(whitePlayer->set[i]);
    draw_piece(blackPlayer->set[i]);
  }
}

void (draw_all_timers)(){
  draw_player_timer_frame(blackPlayer);
  draw_player_timer_frame(whitePlayer);
}

void (update_player_timer)(Player* p){
  clean_player_timer_frame(p);
  draw_player_timer_frame(p);
}

void (draw_player_timer_frame)(Player* p){
  unsigned int seconds = p->time % 60;
  unsigned int minutes = p->time / 60;

  if(p->color == WHITE){
    draw_digit(800, 540, minutes / 10);
    draw_digit(840, 540, minutes % 10);
    draw_digit(880, 540, 10);
    draw_digit(920, 540, seconds / 10);
    draw_digit(960, 540, seconds % 10);
  }

  else{
    draw_digit(800, 160, minutes / 10);
    draw_digit(840, 160, minutes % 10);
    draw_digit(880, 160, 10);
    draw_digit(920, 160, seconds / 10);
    draw_digit(960, 160, seconds % 10);
  }
}

void (clean_player_timer_frame)(Player* p){
  
}
