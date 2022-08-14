#ifndef LETTER_H
#define LETTER_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <string.h>

#include "graphics_card.h"

#include "images/letters/A.xpm"
#include "images/letters/B.xpm"
#include "images/letters/C.xpm"
#include "images/letters/D.xpm"
#include "images/letters/E.xpm"
#include "images/letters/F.xpm"
#include "images/letters/G.xpm"
#include "images/letters/H.xpm"
#include "images/letters/I.xpm"
#include "images/letters/J.xpm"
#include "images/letters/K.xpm"
#include "images/letters/L.xpm"
#include "images/letters/M.xpm"
#include "images/letters/N.xpm"
#include "images/letters/O.xpm"
#include "images/letters/P.xpm"
#include "images/letters/Q.xpm"
#include "images/letters/R.xpm"
#include "images/letters/S.xpm"
#include "images/letters/T.xpm"
#include "images/letters/U.xpm"
#include "images/letters/V.xpm"
#include "images/letters/W.xpm"
#include "images/letters/X.xpm"
#include "images/letters/Y.xpm"
#include "images/letters/Z.xpm"

void (load_all_letters)();

void (load_letter)(char letter);

void (draw_letter)(uint16_t x, uint16_t y, char letter);

void (draw_string)(char *text, uint16_t x, uint16_t y);

void (insert_letter_into_back_frame)(uint16_t x, uint16_t y, char letter);

void (insert_string_into_back_frame)(uint16_t x, uint16_t y, char *text);

#endif

