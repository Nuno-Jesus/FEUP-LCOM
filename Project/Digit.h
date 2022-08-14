#ifndef DIGIT_H
#define DIGIT_H

#include <lcom/lcf.h>
#include <stdint.h>

#include "graphics_card.h"

#include "Menu.h"

#include "images/digits/0.xpm"
#include "images/digits/1.xpm"
#include "images/digits/2.xpm"
#include "images/digits/3.xpm"
#include "images/digits/4.xpm"
#include "images/digits/5.xpm"
#include "images/digits/6.xpm"
#include "images/digits/7.xpm"
#include "images/digits/8.xpm"
#include "images/digits/9.xpm"
#include "images/digits/dots.xpm"

void (load_all_digits)();

void (load_digit)(int number);

void (draw_digit)(uint16_t x, uint16_t y, unsigned int number);

void (clean_digit)(uint16_t x, uint16_t y, unsigned int number);

void (insert_digit_into_back_frame)(uint16_t x, uint16_t y, unsigned int number);

void (remove_digit_from_back_frame)(uint16_t x, uint16_t y, unsigned int number);

#endif
