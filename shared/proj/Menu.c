#include "Menu.h"

#define NUM_MENUS 6

Menu** all_menus;
xpm_image_t background_frame;

//From "graphics_card.c"
extern unsigned int hres;
extern unsigned int vres;

//From "Game.c"
extern struct mouse_ev event;

//From "Player.c"
extern Player* blackPlayer;
extern Player* whitePlayer;

/************ SETTINGS ************/

SettingsBoardStyle boardStyle = CLASSIC;
SettingsPieceStyle pieceStyle = STANDARD;

void (create_all_menus)(){
  all_menus = (Menu**) malloc(NUM_MENUS * sizeof(Menu*));
  if(all_menus == NULL)
    return;

  all_menus[0] = new_menu(MAIN_MENU, 4);
  all_menus[1] = new_menu(PLAY_MENU, 0);
  all_menus[2] = new_menu(HELP_MENU, 1);
  all_menus[3] = new_menu(PAUSE_MENU, 2);
  all_menus[4] = new_menu(SETTINGS_MENU, 4);
  all_menus[5] = new_menu(GAME_OVER_MENU, 2);
}

Menu* (new_menu)(MenuType type, int numButtons){
  Menu* m = (Menu*) malloc(sizeof(Menu));
  if(m == NULL)
    return NULL;

  m->type = type;
  m->numButtons = numButtons;

  if(numButtons != 0){
    m->buttons = (Button**) malloc(numButtons * sizeof(Button*));
      if(m->buttons == NULL)
        return NULL;
  }

  switch(type){
    case MAIN_MENU:
      xpm_load(main_menu_xpm, XPM_8_8_8_8, &(m->img));

      m->buttons[0] = new_button(51, 305, START_BUTTON);
      m->buttons[1] = new_button(51, 416, HELP_BUTTON);
      m->buttons[2] = new_button(51, 527, SETTINGS_BUTTON);
      m->buttons[3] = new_button(51, 638, EXIT_BUTTON);

      break;

    case PLAY_MENU:
      xpm_load(play_menu_xpm, XPM_8_8_8_8, &(m->img));
      break;

    case HELP_MENU:
      break;

    case PAUSE_MENU:
      break;

    case SETTINGS_MENU:
      xpm_load(settings_menu_xpm, XPM_8_8_8_8, &(m->img));

      m->buttons[0] = new_button(55, 255, PIECE_LEFT_ARROW_BUTTON);
      m->buttons[1] = new_button(403, 255, PIECE_RIGHT_ARROW_BUTTON);
      m->buttons[2] = new_button(55, 450, CHESSBOARD_LEFT_ARROW_BUTTON);
      m->buttons[3] = new_button(403, 450, CHESSBOARD_RIGHT_ARROW_BUTTON);
      
      break;

    case GAME_OVER_MENU:
      break;

    default: 
      break;
  }

  return m;
}

void (delete_menu)(Menu* menu){
  for(int i = 0; i < menu->numButtons; i++){
    delete_button(menu->buttons[i]);
  }

  free(menu->buttons);
  free(menu);
}

void (draw_menu)(MenuType type){
  uint32_t* bytes = (uint32_t*) all_menus[type]->img.bytes;

  for(uint16_t i = 0; i < all_menus[type]->img.width; i++){
    for(uint16_t j = 0; j < all_menus[type]->img.height; j++){
      if(*(bytes + i + j * all_menus[type]->img.width) != TRANSPARENCY)
        paint_pixel(i, j, *(bytes + i + j * all_menus[type]->img.width));
    }
  }

  for(int i = 0; i < all_menus[type]->numButtons; i++){
    draw_button(all_menus[type]->buttons[i]);
  }

  insert_menu_into_main_frame(type);
}

void (insert_menu_into_main_frame)(MenuType type){
  switch(type){
    case MAIN_MENU:
      xpm_load(main_menu_xpm, XPM_8_8_8_8, &background_frame);
      break;
    
    case PLAY_MENU:
      xpm_load(play_menu_xpm, XPM_8_8_8_8, &background_frame);
      break;

    case HELP_MENU:
      //xpm_load(help_menu_xpm, XPM_8_8_8_8, &background_frame);
      break;

    case PAUSE_MENU:
      //xpm_load(pause_menu_xpm, XPM_8_8_8_8, &background_frame);
      break;

    case SETTINGS_MENU:
      xpm_load(settings_menu_xpm, XPM_8_8_8_8, &background_frame);
      break;

    case GAME_OVER_MENU:
      //xpm_load(game_over_menu_xpm, XPM_8_8_8_8, &background_frame);
      break;

    default: 
      break;
  }

  for(int i = 0; i < all_menus[type]->numButtons; i++){
    insert_button_into_back_frame(all_menus[type]->buttons[i]);
  }
}

void (toggle_highlight_button)(MenuType menuType, ButtonType buttonType){
  //If the button was pressed, toggles red highlight
  if(event.type == LB_PRESSED){
    for(int i = 0; i < all_menus[menuType]->numButtons; i++){
      if(all_menus[menuType]->buttons[i]->type == buttonType){
        all_menus[menuType]->buttons[i]->hasBeenPressed = true;
        all_menus[menuType]->buttons[i]->mouseIsOver = false;
        
        insert_button_into_back_frame(all_menus[menuType]->buttons[i]);
        draw_button(all_menus[menuType]->buttons[i]);
        break;
      }
    }
  }

  //Otherwise only toggles blue highlight
  else{
    for(int i = 0; i < all_menus[menuType]->numButtons; i++){
      if(all_menus[menuType]->buttons[i]->type == buttonType){
        all_menus[menuType]->buttons[i]->mouseIsOver = true;
        all_menus[menuType]->buttons[i]->hasBeenPressed = false;

        insert_button_into_back_frame(all_menus[menuType]->buttons[i]);
        draw_button(all_menus[menuType]->buttons[i]);
        break;
      }
    }
  }
}

void (untoggle_button_highlight)(MenuType menuType, ButtonType buttonType){
  //If the mouse is over the button but left button was released untoggles red highlight
  if(event.type == LB_RELEASED){
    for(int i = 0; i < all_menus[menuType]->numButtons; i++){
      if(all_menus[menuType]->buttons[i]->type == buttonType){
        all_menus[menuType]->buttons[i]->hasBeenPressed = false;
        all_menus[menuType]->buttons[i]->mouseIsOver = true;
        
        insert_button_into_back_frame(all_menus[menuType]->buttons[i]);
        draw_button(all_menus[menuType]->buttons[i]);

        break;
      }
    }
  }

  //Otherwise, the cursor is off the button and untoggles all highlights
  else{
    for(int i = 0; i < all_menus[menuType]->numButtons; i++){
      if(all_menus[menuType]->buttons[i]->type == buttonType){
        all_menus[menuType]->buttons[i]->hasBeenPressed = false;
        all_menus[menuType]->buttons[i]->mouseIsOver = false;
        
        insert_button_into_back_frame(all_menus[menuType]->buttons[i]);
        draw_button(all_menus[menuType]->buttons[i]);

        break;
      }
    }
  }
}

/*************************** SETTINGS MENU SPECIFIC ***************************/

void (draw_piece_style)(){
  clean_settings_display();
  clean_settings_piece_string();

  Piece* display1 = new_piece(WHITE, PAWN, 575, 184, 0, 0, 0);
  Piece* display2 = new_piece(WHITE, KNIGHT, 720, 184, 0, 0, 0);
  Piece* display3 = new_piece(WHITE, BISHOP, 865, 184, 0, 0, 0);
  Piece* display4 = new_piece(WHITE, ROOK, 575, 335, 0, 0, 0);
  Piece* display5 = new_piece(WHITE, QUEEN, 720, 335, 0, 0, 0);
  Piece* display6 = new_piece(WHITE, KING, 865, 335, 0, 0, 0);

  Piece* display7 = new_piece(BLACK, PAWN, 575, 486, 0, 0, 0);
  Piece* display8 = new_piece(BLACK, KNIGHT, 720, 486, 0, 0, 0);
  Piece* display9 = new_piece(BLACK, BISHOP, 865, 486, 0, 0, 0);
  Piece* display10 = new_piece(BLACK, ROOK, 575, 637, 0, 0, 0);
  Piece* display11 = new_piece(BLACK, QUEEN, 720, 637, 0, 0, 0);
  Piece* display12 = new_piece(BLACK, KING, 865, 637, 0, 0, 0);

  switch(pieceStyle){
    case STANDARD: draw_string("STANDARD", 120, 250); break;
    case MIDNIGHT: draw_string("MIDNIGHT", 120, 250); break;
    default: break;
  }

  draw_piece(display1); delete_piece(display1);
  draw_piece(display2); delete_piece(display2);
  draw_piece(display3); delete_piece(display3);
  draw_piece(display4); delete_piece(display4);
  draw_piece(display5); delete_piece(display5);
  draw_piece(display6); delete_piece(display6);

  draw_piece(display7); delete_piece(display7);
  draw_piece(display8); delete_piece(display8);
  draw_piece(display9); delete_piece(display9);
  draw_piece(display10); delete_piece(display10);
  draw_piece(display11); delete_piece(display11);
  draw_piece(display12); delete_piece(display12);
}

void (draw_chessboard_style)(){
  clean_settings_display();
  clean_settings_chessboard_string();

  Chessboard* display = new_chessboard(590, 270);
  
  switch(boardStyle){
    case CLASSIC:
      draw_string("CLASSIC", 135, 445);
      xpm_load(settings_chessboard_xpm, XPM_8_8_8_8, &(display->img));
      break;

    case CHERRY:
      draw_string("CHERRY", 155, 445);
      xpm_load(settings_cherry_chessboard_xpm, XPM_8_8_8_8, &(display->img));
      break;
    
    case GREY:
      draw_string("GREY", 175, 445);
      xpm_load(settings_grey_chessboard_xpm, XPM_8_8_8_8, &(display->img));
      break;
    
    case MINT:
      draw_string("MINT", 175, 445);
      xpm_load(settings_mint_chessboard_xpm, XPM_8_8_8_8, &(display->img));
      break;

    case GLASS:
      draw_string("GLASS", 165, 445);
      xpm_load(settings_glass_chessboard_xpm, XPM_8_8_8_8, &(display->img));
      break;

    default:
      break;
  }

  draw_chessboard(display);
  delete_chessboard(display);
}

void (clean_settings_display)(){
  uint32_t* back_frame;
  uint32_t* menu_bytes;

  back_frame = (uint32_t*)background_frame.bytes;
  menu_bytes = (uint32_t*)all_menus[SETTINGS_MENU]->img.bytes;

  for(uint16_t i = background_frame.width/2; i < background_frame.width; i++){
    for(uint16_t j = 0; j < background_frame.height; j++){
      if(*(menu_bytes + i + j*background_frame.height) != TRANSPARENCY){
        *(back_frame + i + j*background_frame.width) = *(menu_bytes + i + j*background_frame.width);
        paint_pixel(i, j, *(back_frame + i + j*background_frame.width));
      }
    }
  }
}

void (clean_settings_piece_string)(){
  uint32_t* back_frame;
  uint32_t* menu_bytes;

  back_frame = (uint32_t*)background_frame.bytes;
  menu_bytes = (uint32_t*)all_menus[SETTINGS_MENU]->img.bytes;

  for(uint16_t i = 135; i < 390; i++){
    for(uint16_t j = 240; j < 340; j++){
      if(*(menu_bytes + i + j*background_frame.height) != TRANSPARENCY){
        *(back_frame + i + j*background_frame.width) = *(menu_bytes + i + j*background_frame.width);
        paint_pixel(i, j, *(back_frame + i + j*background_frame.width));
      }
    }
  }
}

void (clean_settings_chessboard_string)(){
  uint32_t* back_frame;
  uint32_t* menu_bytes;

  back_frame = (uint32_t*)background_frame.bytes;
  menu_bytes = (uint32_t*)all_menus[SETTINGS_MENU]->img.bytes;

  for(uint16_t i = 135; i < 390; i++){
    for(uint16_t j = 435; j < 535; j++){
      if(*(menu_bytes + i + j*background_frame.height) != TRANSPARENCY){
        *(back_frame + i + j*background_frame.width) = *(menu_bytes + i + j*background_frame.width);
        paint_pixel(i, j, *(back_frame + i + j*background_frame.width));
      }
    }
  }
}

/***************************** PLAY MENU SPECIFIC *****************************/

void (insert_chessboard_into_play_menu)(xpm_image_t img){
  uint32_t *bytes;
  uint32_t *menu_frame;

  bytes = (uint32_t*)img.bytes;
  menu_frame = (uint32_t*)all_menus[PLAY_MENU]->img.bytes;

  for(uint16_t i = 0; i < img.width; i++){
    for(uint16_t j = 0; j < img.height; j++){
      if(*(bytes + i + j * img.width) != TRANSPARENCY)
        *(menu_frame + i + j*all_menus[PLAY_MENU]->img.width) = *(bytes + i + j*img.width);
    }
  }
}

void (remove_piece_from_back_frame)(xpm_image_t img, uint16_t x, uint16_t y){
  uint32_t* back_frame;
  uint32_t* menu_bytes;

  back_frame = (uint32_t*)background_frame.bytes;
  menu_bytes = (uint32_t*)all_menus[PLAY_MENU]->img.bytes;

  for(uint16_t i = 0; i < img.width; i++){
    for(uint16_t j = 0; j < img.height; j++){
      *(back_frame + (x + i) + (y + j)*background_frame.width) = *(menu_bytes + (x + i) + (y + j)*all_menus[PLAY_MENU]->img.width);
    }
  }
}




