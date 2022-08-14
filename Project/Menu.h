#ifndef MENU_H
#define MENU_H

#include <lcom/lcf.h>

#include "Button.h"
#include "Piece.h"
#include "Player.h"
#include "Chessboard.h"
#include "Letter.h"
#include "SettingsStyle.h"

#include "images/menus/main_menu.xpm"
#include "images/menus/play_menu.xpm"
#include "images/menus/settings_menu.xpm"

#include "images/menus/settings_chessboard.xpm"
#include "images/menus/settings_glass_chessboard.xpm"
#include "images/menus/settings_cherry_chessboard.xpm"
#include "images/menus/settings_grey_chessboard.xpm"
#include "images/menus/settings_mint_chessboard.xpm"

#include "images/menus/dark_clock.xpm"


typedef enum MenuType_{
  MAIN_MENU,
  PLAY_MENU,
  HELP_MENU,
  PAUSE_MENU,
  SETTINGS_MENU,
  GAME_OVER_MENU
} MenuType;

typedef struct Menu_{
  MenuType type;
  
  int numButtons;
  Button** buttons;

  xpm_image_t img;
} Menu;

void (create_all_menus)();

Menu* (new_menu)(MenuType type, int numButtons);

void (delete_menu)(Menu* menu);

void (draw_menu)(MenuType type);

void (insert_menu_into_main_frame)(MenuType type);

void (toggle_highlight_button)(MenuType menuType, ButtonType buttonType);

void (untoggle_button_highlight)(MenuType menuType, ButtonType buttonType);

/*************************** SETTINGS MENU SPECIFIC ***************************/

void (draw_piece_style)();

void (draw_chessboard_style)();

void (clean_settings_display)();

void (clean_settings_piece_string)();

void (clean_settings_chessboard_string)();

/***************************** PLAY MENU SPECIFIC *****************************/

void (insert_chessboard_into_play_menu)(xpm_image_t img);

void (remove_piece_from_back_frame)(xpm_image_t img, uint16_t x, uint16_t y);

#endif
