#include "Game.h"

//From "timer.c"
extern unsigned int counter;

//From "kbc.c" 
extern uint8_t scancode, status;

//From "mouse.c"
extern Cursor *cursor;

//Mouse related variables
struct packet pack;
struct mouse_ev event;

//Game state machine, with "MAINMENU" being the initial state
GameStateMachine game_state = MAINMENU; 

//From "Menu.c"
extern SettingsBoardStyle boardStyle;
extern SettingsPieceStyle pieceStyle;

//Current game chessboard
Chessboard* cb;

//From "Player.c"
extern Player* whitePlayer;
extern Player* blackPlayer;


int (game_loop)(){
  int ipc_status, r;
  message msg;

  uint32_t irq_set_timer = BIT(TIMER0_IRQ); 
  uint32_t irq_set_keyboard = BIT(KBD_IRQ); 
  uint32_t irq_set_mouse = BIT(MOUSE_IRQ); 
  
  uint8_t bit_no_timer;
  uint8_t bit_no_keyboard;
  uint8_t bit_no_mouse;

  uint8_t scancode_bytes[2];
  uint8_t packet_bytes[3];

  int scancode_size = 1;
  int packet_size = 1;

  create_all_menus();
  draw_menu(MAIN_MENU);

  load_all_letters();
  load_all_digits();

  new_cursor(30, 30);
  draw_cursor_frame();
  
  if(timer_subscribe_int(&bit_no_timer) != OK) return TIMER_SET_POLICY_ERROR;
  if(kbd_subscribe_int(&bit_no_keyboard) != OK) return KEYBOARD_SET_POLICY_ERROR;
  if(mouse_enable_data_report() != OK) return MOUSE_ENABLE_DATA_ERROR;
  if(mouse_subscribe_int(&bit_no_mouse) != OK) return MOUSE_SET_POLICY_ERROR;
  
  while(game_state != EXIT){
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: 

/******************************** KEYBOARD ********************************/
          if (msg.m_notify.interrupts & irq_set_keyboard) {
            kbc_ih();

            scancode_bytes[scancode_size - 1] = scancode;

            if(scancode_size == 1){
              if(scancode == FIRST_BYTE_OUT_OF_TWO)
                scancode_size++;

              else
                interrupt_handler_multiplexer(KEYBOARD);
            }

            else{
              interrupt_handler_multiplexer(KEYBOARD);
              scancode_size--;
            }
          }

/********************************** MOUSE *********************************/
          if (msg.m_notify.interrupts & irq_set_mouse) {
            mouse_ih();
              
            packet_bytes[packet_size - 1] = scancode;

            if(packet_size == 1 && (scancode & FIRST_BYTE)){
              packet_size++;
            }

            else if(packet_size == 2){
              packet_size++;
            }

            else if(packet_size == 3){
              for(int i = 0; i < 3; i++)
                pack.bytes[i] = packet_bytes[i];

              mouse_assemble_packet(&pack);
              interrupt_handler_multiplexer(MOUSE);

              packet_size = 1; 
            }
          }
        
/********************************** TIMER *********************************/
          if (msg.m_notify.interrupts & irq_set_timer){
            timer_int_handler();
            interrupt_handler_multiplexer(TIMER);
          }

          break;

        default:
          break; 
      }
    }
  }

  if(mouse_unsubscribe_int() != OK) return MOUSE_REMOVE_POLICY_ERROR;
  if(mouse_disable_data_report() != OK) return MOUSE_DISABLE_DATA_ERROR;
  if(kbd_unsubscribe_int() != OK) return KEYBOARD_REMOVE_POLICY_ERROR;
  if(timer_unsubscribe_int() != OK) return TIMER_REMOVE_POLICY_ERROR;

  return OK;
}

void (interrupt_handler_multiplexer)(Device driver){
  switch(game_state){
    case MAINMENU:
      main_menu_interrupt_handler(driver);
      break;

    case PLAY:
      play_interrupt_handler(driver);
      break;

    case HELP:
      help_interrupt_handler(driver);
      break;

    case SETTINGS:
      settings_interrupt_handler(driver);
      break;

    case PAUSE:
      pause_menu_interrupt_handler(driver);
      break;

    case EXIT:
      break;

    default:
      break;
  }
}

void (main_menu_interrupt_handler)(Device driver){
  static bool wasOverStartButton = false;
  static bool wasOverHelpButton = false;
  static bool wasOverSettingsButton = false;
  static bool wasOverExitButton = false;

  static bool hasPressedStartButton = false;
  static bool hasPressedHelpButton = false;
  static bool hasPressedSettingsButton = false;
  static bool hasPressedExitButton = false;

  switch(driver){
    //Every timer interrupt is used to call "cursor_collisions_in_main_menu" and check for cursor collisions with main menu buttons
    case TIMER:
      switch(cursor_collisions_in_main_menu()){
        case OVER_START_BUTTON:
          //If the cursor collided with start button for "the first time"
          if(!wasOverStartButton){
            wasOverStartButton = true;
            toggle_highlight_button(MAIN_MENU, START_BUTTON);
          }

          //Else, if the cursor is above the start button and the left button was pressed 
          else if(wasOverStartButton && event.type == LB_PRESSED){
            hasPressedStartButton = true;
            toggle_highlight_button(MAIN_MENU, START_BUTTON);
          }

          //The same as above, but when the left button is released
          else if(wasOverStartButton && hasPressedStartButton){
            untoggle_button_highlight(MAIN_MENU, START_BUTTON);
            hasPressedStartButton = false;

            cursor->isAboveButton = false;
            game_state = PLAY;
            draw_menu(PLAY_MENU);
          }

          break;

        case OVER_HELP_BUTTON:
          //If the cursor collided with settings button for "the first time"
          if(!wasOverHelpButton){
            wasOverHelpButton = true;
            toggle_highlight_button(MAIN_MENU, HELP_BUTTON);
          }

          //Else, if the cursor is above the help button and the left button was pressed 
          else if(wasOverHelpButton && event.type == LB_PRESSED){
            hasPressedHelpButton = true;
            toggle_highlight_button(MAIN_MENU, HELP_BUTTON);
          }

          //The same as above, but when the left button is released
          else if(wasOverHelpButton && hasPressedHelpButton){
            untoggle_button_highlight(MAIN_MENU, HELP_BUTTON);
            hasPressedHelpButton = false;

            cursor->isAboveButton = false;
          }
          
          break;

        case OVER_SETTINGS_BUTTON:
          //If the cursor collided with settings button for "the first time"
          if(!wasOverSettingsButton){
            wasOverSettingsButton = true;
            toggle_highlight_button(MAIN_MENU, SETTINGS_BUTTON);
          }

          //Else, if the cursor is above the settings button and the left button was pressed 
          else if(wasOverSettingsButton && event.type == LB_PRESSED){
            hasPressedSettingsButton = true;
            toggle_highlight_button(MAIN_MENU, SETTINGS_BUTTON);
          }

          //The same as above, but when the left button is released
          else if(wasOverSettingsButton && hasPressedSettingsButton){
            untoggle_button_highlight(MAIN_MENU, SETTINGS_BUTTON);
            hasPressedSettingsButton = false;
            cursor->isAboveButton = false;
            
            game_state = SETTINGS;
            draw_menu(SETTINGS_MENU);
          }
          
          break;

        case OVER_EXIT_BUTTON:
          //If the cursor collided with exit button for "the first time"
          if(!wasOverExitButton){
            wasOverExitButton = true;
            toggle_highlight_button(MAIN_MENU, EXIT_BUTTON);
          }

          //Else, if the cursor is above the exit button and the left button was pressed 
          else if(wasOverExitButton && event.type == LB_PRESSED){
            hasPressedExitButton = true;
            toggle_highlight_button(MAIN_MENU, EXIT_BUTTON);
          }

          //The same as above, but when the left button is released
          else if(wasOverExitButton && hasPressedExitButton){
            untoggle_button_highlight(MAIN_MENU, EXIT_BUTTON);
            hasPressedExitButton = false;
            cursor->isAboveButton = false;
            
            game_state = EXIT;
          }
          
          break;

        //In case the mouse cursor is no longer above any of the buttons
        case NO_COLLISION:
          //Untoggles button highlight, according to the button the cursor was on before
          if(wasOverStartButton){
            untoggle_button_highlight(MAIN_MENU, START_BUTTON);
            wasOverStartButton = false;
          }

          else if(wasOverHelpButton){
            untoggle_button_highlight(MAIN_MENU, HELP_BUTTON);
            wasOverHelpButton = false;
          }

          else if(wasOverSettingsButton){
            untoggle_button_highlight(MAIN_MENU, SETTINGS_BUTTON);
            wasOverSettingsButton = false;
          }

          else if(wasOverExitButton){
            untoggle_button_highlight(MAIN_MENU, EXIT_BUTTON);
            wasOverExitButton = false;
          }

          break;

        default:
          break;
      }

      break;

    case KEYBOARD:
      break;

    case MOUSE:
      update_cursor_position(&pack);
      event = mouse_events(&pack);
      break;

    default:
      break;
  }
}

void (play_interrupt_handler)(Device driver){ 
  static Player* currPlayer = NULL; 
  static Piece* currPiece = NULL;
  static Piece* oldPiece = NULL;

  static int selectedMoveX = 0;
  static int selectedMoveY = 0;

  static bool isFirstTime = true;
  static bool hasSelectedPiece = false;
  static bool moveIsValid = false;
  static bool validMoves[8][8] = 
  {
    {false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false},
    {false, false, false, false, false, false, false, false}
  };

  if(isFirstTime){
    cb = new_chessboard(0, 0);
    draw_chessboard(cb);
    insert_chessboard_into_back_frame(cb);
    insert_chessboard_into_play_menu(cb->img);

    create_all_players();
    draw_all_player_pieces();
    draw_all_timers();

    empty_chessboard(cb);
    fill_chessboard(cb);

    currPlayer = whitePlayer;
    isFirstTime = false;
  } 
  
  switch(driver){
    case TIMER:
      //If didn't select a piece yet
      if(!hasSelectedPiece){
        currPiece = cursor_collided_on_chessboard(cb);
        //If the mouse cursor collided with a piece the player can move, highlights it
        if(currPiece != NULL && currPiece->color == currPlayer->color){
          toggle_piece_blue_highlight(currPiece);
          
          if(event.type == LB_PRESSED){
            get_valid_moves(currPiece, validMoves, cb->board);
            display_valid_moves(validMoves, cb->board);

            hasSelectedPiece = true;
          }
        }

        //If the cursor moved towards another piece, untoggles the previous highlight
        if(currPiece != oldPiece && oldPiece != NULL){
          untoggle_piece_blue_highlight(oldPiece);
          remove_piece_from_back_frame(oldPiece->img, oldPiece->x, oldPiece->y);
          clean_piece(oldPiece);
          draw_piece(oldPiece);
        }

        oldPiece = currPiece;

      }

      //If the user selected a piece
      else{
        //And clicked on a square in the board
        if(event.type == LB_PRESSED){
          moveIsValid = validate_move(validMoves, &selectedMoveX, &selectedMoveY); 
          if(moveIsValid){
            clean_valid_moves(validMoves, cb->board);
            reset_valid_moves(validMoves);

            remove_piece_from_back_frame(currPiece->img, currPiece->x, currPiece->y);
            move_piece(currPiece, cb->board[selectedMoveY][selectedMoveX]);
            
            hasSelectedPiece = false;
            moveIsValid = false;
            
            //! The following line is pretty important, since if not written, the same piece would be selected automatically 
            event.type = MOUSE_MOV;
          }
        }
       
      }

      break;

    case KEYBOARD:
      if(scancode == ESC){
        delete_chessboard(cb);
        //remove_chessboard_from_play_menu();
        delete_all_players();

        reset_valid_moves(validMoves);

        currPlayer = NULL;
        isFirstTime = true;
        hasSelectedPiece = false;
        moveIsValid = false;
        selectedMoveX = 0;
        selectedMoveY = 0;

        game_state = MAINMENU;
        draw_menu(MAIN_MENU);
      }
      
      break;
      
    case MOUSE:
      update_cursor_position(&pack);
      event = mouse_events(&pack);
      break;

    default:
      break;
  }
}

void (help_interrupt_handler)(Device driver){
  switch(driver){
    case TIMER:
      break;

    case KEYBOARD:
      break;
      
    case MOUSE:
      break;

    default:
      break;
  }
}

void (settings_interrupt_handler)(Device driver){
  static bool wasOverPieceLeftArrowButton = false;
  static bool wasOverPieceRightArrowButton = false;
  static bool wasOverChessboardLeftArrowButton = false;
  static bool wasOverChessboardRightArrowButton = false;

  static bool hasPressedPieceLeftArrowButton = false;
  static bool hasPressedPieceRightArrowButton = false;
  static bool hasPressedChessboardLeftArrowButton = false;
  static bool hasPressedChessboardRightArrowButton = false;
  
  static bool hasDrawnPieceStyleBefore = false;
  static bool hasDrawnChessboardStyleBefore = false;

  static bool isFirstTime = true;

  if(isFirstTime){
    switch(pieceStyle){
      case STANDARD:
        draw_string("STANDARD", 120, 250);
        break;

      case MIDNIGHT:
        draw_string("MIDNIGHT", 120, 250);
        break;

      default:
        break;
    }

    switch(boardStyle){
      case CLASSIC:
        draw_string("CLASSIC", 135, 445);
        break;
      
      case CHERRY:
        draw_string("CHERRY", 155, 445);
        break;
      
      case GREY:
        draw_string("GREY", 175, 445);
        break;
      
      case MINT:
        draw_string("MINT", 175, 445);
        break;

      case GLASS:
        draw_string("GLASS", 165, 445);
        break;

      default:
        break;
    }

    isFirstTime = false;
  }


  switch(driver){
    case TIMER:
      switch(cursor_collisions_in_settings_menu()){
        case OVER_PIECE_LEFT_ARROW_BUTTON:
          if(!wasOverPieceLeftArrowButton){
            wasOverPieceLeftArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, PIECE_LEFT_ARROW_BUTTON);
            
            if(!hasDrawnPieceStyleBefore){
              clean_settings_display();
              draw_piece_style();
              
              hasDrawnChessboardStyleBefore = false;
              hasDrawnPieceStyleBefore = true;
            }
          }

          else if(wasOverPieceLeftArrowButton && event.type == LB_PRESSED){
            hasPressedPieceLeftArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, PIECE_LEFT_ARROW_BUTTON);
          }

          else if(wasOverPieceLeftArrowButton && hasPressedPieceLeftArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, PIECE_LEFT_ARROW_BUTTON);
            hasPressedPieceLeftArrowButton = false;

            if(pieceStyle != STANDARD){
              pieceStyle--;
              
              clean_settings_display();
              draw_piece_style();
                
              hasDrawnChessboardStyleBefore = false;
              hasDrawnPieceStyleBefore = true;
            }

          }

          break;

        case OVER_PIECE_RIGHT_ARROW_BUTTON:
          if(!wasOverPieceRightArrowButton){
            wasOverPieceRightArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, PIECE_RIGHT_ARROW_BUTTON);
            
            if(!hasDrawnPieceStyleBefore){
              clean_settings_display();
              draw_piece_style();
              
              hasDrawnChessboardStyleBefore = false;
              hasDrawnPieceStyleBefore = true;
            }
          }

          else if(wasOverPieceRightArrowButton && event.type == LB_PRESSED){
            hasPressedPieceRightArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, PIECE_RIGHT_ARROW_BUTTON);
          }

          else if(wasOverPieceRightArrowButton && hasPressedPieceRightArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, PIECE_RIGHT_ARROW_BUTTON);
            hasPressedPieceRightArrowButton = false;

            if(pieceStyle != MIDNIGHT){
              pieceStyle++;
              
              clean_settings_display();
              draw_piece_style();
                
              hasDrawnChessboardStyleBefore = false;
              hasDrawnPieceStyleBefore = true;
            }
          }

          break;

        case OVER_CHESSBOARD_LEFT_ARROW_BUTTON:
          if(!wasOverChessboardLeftArrowButton){
            wasOverChessboardLeftArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, CHESSBOARD_LEFT_ARROW_BUTTON);
            
            if(!hasDrawnChessboardStyleBefore){
              clean_settings_display();
              draw_chessboard_style();
              
              hasDrawnPieceStyleBefore = false;
              hasDrawnChessboardStyleBefore = true;
            }
          }

          else if(wasOverChessboardLeftArrowButton && event.type == LB_PRESSED){
            hasPressedChessboardLeftArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, CHESSBOARD_LEFT_ARROW_BUTTON);
          }

          else if(wasOverChessboardLeftArrowButton && hasPressedChessboardLeftArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, CHESSBOARD_LEFT_ARROW_BUTTON);
            hasPressedChessboardLeftArrowButton = false;

            if(boardStyle != CLASSIC){
              boardStyle--;
              
              clean_settings_display();
              draw_chessboard_style();
                
              hasDrawnPieceStyleBefore = false;
              hasDrawnChessboardStyleBefore = true;
            }
          }

          break;

        case OVER_CHESSBOARD_RIGHT_ARROW_BUTTON:
          if(!wasOverChessboardRightArrowButton){
            wasOverChessboardRightArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, CHESSBOARD_RIGHT_ARROW_BUTTON);
            
            if(!hasDrawnChessboardStyleBefore){
              clean_settings_display();
              draw_chessboard_style();
              
              hasDrawnPieceStyleBefore = false;
              hasDrawnChessboardStyleBefore = true;
            }
          }

          else if(wasOverChessboardRightArrowButton && event.type == LB_PRESSED){
            hasPressedChessboardRightArrowButton = true;
            toggle_highlight_button(SETTINGS_MENU, CHESSBOARD_RIGHT_ARROW_BUTTON);
          }

          else if(wasOverChessboardRightArrowButton && hasPressedChessboardRightArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, CHESSBOARD_RIGHT_ARROW_BUTTON);
            hasPressedChessboardRightArrowButton = false;

            if(boardStyle != GLASS){
              boardStyle++;
              
              clean_settings_display();
              draw_chessboard_style();
                
              hasDrawnPieceStyleBefore = false;
              hasDrawnChessboardStyleBefore = true;
            }
          }

          break;

        case NO_COLLISION:
          if(wasOverPieceLeftArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, PIECE_LEFT_ARROW_BUTTON);
            wasOverPieceLeftArrowButton = false;
          }

          else if(wasOverPieceRightArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, PIECE_RIGHT_ARROW_BUTTON);
            wasOverPieceRightArrowButton = false;
          }

          else if(wasOverChessboardLeftArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, CHESSBOARD_LEFT_ARROW_BUTTON);
            wasOverChessboardLeftArrowButton = false;
          }

          else if(wasOverChessboardRightArrowButton){
            untoggle_button_highlight(SETTINGS_MENU, CHESSBOARD_RIGHT_ARROW_BUTTON);
            wasOverChessboardRightArrowButton = false;
          }

          
          break;

        default:
          break;
      }

      break;

    case KEYBOARD:
      if(scancode == ESC){
        game_state = MAINMENU;
        draw_menu(MAIN_MENU);
        isFirstTime = true;
      }

      break;
      
    case MOUSE:
      update_cursor_position(&pack);
      event = mouse_events(&pack);
      break;

    default:
      break;
  }
}

void (pause_menu_interrupt_handler)(Device driver){
  switch(driver){
    case TIMER:
      break;

    case KEYBOARD:
      break;
      
    case MOUSE:
      break;

    default:
      break;
  }
}
