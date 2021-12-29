#include "Piece.h"

//From "Menu.c"
extern xpm_image_t background_frame;
extern SettingsPieceStyle pieceStyle;

Piece* (new_piece)(PieceColor color, PieceType type, int x, int y, int posX, int posY, unsigned int ID){
  Piece* p = (Piece*)malloc(sizeof(Piece));
  if(p == NULL)
    return NULL;

  p->color = color;
  p->type = type;
  p->ID = ID;
  p->x = x;
  p->y = y;
  p->posX = posX;
  p->posY = posY;
  p->isDead = false;
  p->wasPromoted = false;
  p->canBePromoted = false;

  load_piece_xpm(p);

  return p;
}

void (load_piece_xpm)(Piece *p){
  switch(pieceStyle){
    case STANDARD: load_piece_standard_xpm(p); break;
    case MIDNIGHT: load_piece_midnight_xpm(p); break;
    default: break;
  }
}

void (load_piece_standard_xpm)(Piece *p){
  switch(p->type){
    case PAWN:
      if(p->color == WHITE)
        xpm_load(white_pawn_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(black_pawn_xpm, XPM_8_8_8_8, &(p->img));

      break;

    case KNIGHT:
      if(p->color == WHITE)
        xpm_load(white_knight_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(black_knight_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case BISHOP:
      if(p->color == WHITE)
        xpm_load(white_bishop_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(black_bishop_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case ROOK:
      if(p->color == WHITE)
        xpm_load(white_rook_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(black_rook_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case QUEEN:
      if(p->color == WHITE)
        xpm_load(white_queen_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(black_queen_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case KING:
      if(p->color == WHITE)
        xpm_load(white_king_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(black_king_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case EMPTY:
      xpm_load(empty_xpm, XPM_8_8_8_8, &(p->img));
      break;

    default:
      break;

  }
}

void (load_piece_midnight_xpm)(Piece *p){
  /* switch(type){
    case PAWN:
      if(p->color == WHITE)
        xpm_load(midnight_white_pawn_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(midnight_black_pawn_xpm, XPM_8_8_8_8, &(p->img));

      break;

    case KNIGHT:
      if(p->color == WHITE)
        xpm_load(midnight_white_knight_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(midnight_black_knight_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case BISHOP:
      if(p->color == WHITE)
        xpm_load(midnight_white_bishop_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(midnight_black_bishop_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case ROOK:
      if(p->color == WHITE)
        xpm_load(midnight_white_rook_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(midnight_black_rook_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case QUEEN:
      if(p->color == WHITE)
        xpm_load(midnight_white_queen_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(midnight_black_queen_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case KING:
      if(p->color == WHITE)
        xpm_load(midnight_white_king_xpm, XPM_8_8_8_8, &(p->img));
      else
        xpm_load(midnight_black_king_xpm, XPM_8_8_8_8, &(p->img));
      
      break;

    case EMPTY:
      xpm_load(empty_xpm, XPM_8_8_8_8, &(p->img));
      break;

    default:
      break;

  } */
}

void (delete_piece)(Piece* p){
  free(p);
}

void (draw_piece)(Piece* p){
  uint32_t *bytes;

  bytes = (uint32_t*) p->img.bytes;
  for(uint16_t i = 0; i < p->img.width; i++){
    for(uint16_t j = 0; j < p->img.height; j++){
      if(*(bytes + i + j * p->img.width) != TRANSPARENCY)
        paint_pixel(p->x + i, p->y + j, *(bytes + i + j * p->img.width));
    }
  }

  insert_piece_into_back_frame(p);
}

void (clean_piece)(Piece* p){
  uint32_t *back_frame;

  back_frame = (uint32_t*)background_frame.bytes;
  for(uint16_t i = 0; i < p->img.width; i++){
    for(uint16_t j = 0; j < p->img.height; j++){
      if(*(back_frame + i + j * background_frame.width) != TRANSPARENCY)
        paint_pixel(p->x + i, p->y + j, *(back_frame + (p->x + i) + (p->y + j) * background_frame.width));
    }
  }
}

void (move_piece)(Piece* p, Piece* p2){
  clean_piece(p);

  //Swap all content of both pieces
  Piece* aux = p;
  p = p2;
  p2 = aux;

  //Now we swap the coordinates back to their original piece
  swap1(&(p->x), &(p2->x));
  swap1(&(p->y), &(p2->y));
  swap2(&(p->posX), &(p2->posX));
  swap2(&(p->posY), &(p2->posY));
  
  //Only draws the moving piece
  draw_piece(p2);
}

void (insert_piece_into_back_frame)(Piece* p){
  uint32_t *piece_bytes;
  uint32_t *back_frame_bytes;

  piece_bytes = (uint32_t*)p->img.bytes;
  back_frame_bytes = (uint32_t*)background_frame.bytes;

  for(uint16_t i = 0; i < p->img.width; i++){
    for(uint16_t j = 0; j < p->img.height; j++){
      if(*(piece_bytes + i + j * p->img.width) != TRANSPARENCY)
        *(back_frame_bytes + (p->x + i) + (p->y + j) * background_frame.width) = *(piece_bytes + i + j * p->img.width);
    }
  }
}

void (toggle_piece_blue_highlight)(Piece *p){
  uint32_t *bytes;

  bytes = (uint32_t*) p->img.bytes;
  for(uint16_t i = 0; i < p->img.width; i++){
    for(uint16_t j = 0; j < p->img.height; j++){
      if(*(bytes + i + j * p->img.width) == TRANSPARENCY){
        *(bytes + i + j * p->img.width) = BLUE;
        paint_pixel(p->x + i, p->y + j, *(bytes + i + j * p->img.width));
      }
    }
  }

  insert_piece_into_back_frame(p);
}

void (untoggle_piece_blue_highlight)(Piece* p){
  uint32_t *bytes;

  bytes = (uint32_t*) p->img.bytes;
  for(uint16_t i = 0; i < p->img.width; i++){
    for(uint16_t j = 0; j < p->img.height; j++){
      if(*(bytes + i + j * p->img.width) == BLUE){
        *(bytes + i + j * p->img.width) = TRANSPARENCY;
      }
    }
  }
}

void (toggle_piece_red_highlight)(Piece* p){
  uint32_t *bytes;

  bytes = (uint32_t*) p->img.bytes;
  for(uint16_t i = 0; i < p->img.width; i++){
    for(uint16_t j = 0; j < p->img.height; j++){
      if(*(bytes + i + j * p->img.width) == TRANSPARENCY){
        *(bytes + i + j * p->img.width) = RED;
        paint_pixel(p->x + i, p->y + j, *(bytes + i + j * p->img.width));
      }
    }
  }

  insert_piece_into_back_frame(p);
}

void (untoggle_piece_red_highlight)(Piece* p){
  uint32_t *bytes;

  bytes = (uint32_t*) p->img.bytes;
  for(uint16_t i = 0; i < p->img.width; i++){
    for(uint16_t j = 0; j < p->img.height; j++){
      if(*(bytes + i + j * p->img.width) == RED){
        *(bytes + i + j * p->img.width) = TRANSPARENCY;
      }
    }
  }
}
