#include "Chessboard.h"

//From "Menu.c"
extern xpm_image_t background_frame;
extern SettingsBoardStyle boardStyle;

//From "Player.c"
extern Player* blackPlayer;
extern Player* whitePlayer;

//From "Piece.c"
extern Piece* empty_piece;

Chessboard* (new_chessboard)(uint16_t x, uint16_t y){
  Chessboard* cb = (Chessboard*)malloc(sizeof(Chessboard));
  if(cb == NULL)
    return NULL;

  cb->x = x;
  cb->y = y;

  load_board_xpm(cb);

  return cb;
}

void (delete_chessboard)(Chessboard* cb){
  for(int i = 0; i < DIM; i++){
    for(int k = 0; k < DIM; k++){
      delete_piece(cb->board[i][k]);
    }

    free(cb->board[i]);
  }

  free(cb->board);
  free(cb);
}

void (empty_chessboard)(Chessboard* cb){
  for(int i = 0; i < DIM; i++){
    for(int k = 0; k < DIM; k++){
      cb->board[i][k] = new_piece(NO_COLOR, EMPTY, FIRST_X + k*86, FIRST_Y + i*86, k, i, 0);
    }
  }
}

void (load_board_xpm)(Chessboard* cb){
  switch(boardStyle){
    case CLASSIC:
      xpm_load(chessboard_xpm, XPM_8_8_8_8, &(cb->img));
      break;

    case CHERRY:
      xpm_load(cherry_chessboard_xpm, XPM_8_8_8_8, &(cb->img));
      break;

    case GREY:
      xpm_load(grey_chessboard_xpm, XPM_8_8_8_8, &(cb->img));
      break;
    
    case MINT:
      xpm_load(mint_chessboard_xpm, XPM_8_8_8_8, &(cb->img));
      break;

    case GLASS:
      xpm_load(glass_chessboard_xpm, XPM_8_8_8_8, &(cb->img));
      break;
    
    default:
      break;
  }
}

void (fill_chessboard)(Chessboard* cb){
  int x, y;
  
  for(int i = 0; i < NUMPIECES; i++){
    x = whitePlayer->set[i]->posX;
    y = whitePlayer->set[i]->posY;

    if(!whitePlayer->set[i]->isDead)
      cb->board[y][x] = whitePlayer->set[i];

    x = blackPlayer->set[i]->posX;
    y = blackPlayer->set[i]->posY;

    if(!blackPlayer->set[i]->isDead)
      cb->board[y][x] = blackPlayer->set[i];
  }
}

void (draw_chessboard)(Chessboard* cb){
  uint32_t *bytes;

  bytes = (uint32_t*) cb->img.bytes;
  for(uint16_t i = 0; i < cb->img.width; i++){
    for(uint16_t j = 0; j < cb->img.height; j++){
      if(*(bytes + i + j * cb->img.width) != TRANSPARENCY)
        paint_pixel(cb->x + i, cb->y + j, *(bytes + i + j * cb->img.width));
    }
  }
}

void (insert_chessboard_into_back_frame)(Chessboard* cb){
  uint32_t *bytes;
  uint32_t *back_frame;

  bytes = (uint32_t*)cb->img.bytes;
  back_frame = (uint32_t*)background_frame.bytes;

  for(uint16_t i = 0; i < cb->img.width; i++){
    for(uint16_t j = 0; j < cb->img.height; j++){
      if(*(bytes + i + j * cb->img.width) != TRANSPARENCY)
        *(back_frame + (cb->x + i) + (cb->y + j)*background_frame.width) = *(bytes + i + j*cb->img.width);
    }
  }
}

