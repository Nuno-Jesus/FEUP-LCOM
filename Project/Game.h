#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include <stdint.h>

#include <lcom/timer.h>
#include "kbd.h"
#include "mouse.h"
#include "graphics_card.h"

#include "Menu.h"
#include "Letter.h"
#include "Chess.h"

#include "i8042.h"
#include "i8254.h"

typedef enum Error_{
  TIMER_SET_POLICY_ERROR, KEYBOARD_SET_POLICY_ERROR, MOUSE_ENABLE_DATA_ERROR, MOUSE_SET_POLICY_ERROR,
  TIMER_REMOVE_POLICY_ERROR, KEYBOARD_REMOVE_POLICY_ERROR, MOUSE_DISABLE_DATA_ERROR, MOUSE_REMOVE_POLICY_ERROR,
} Error;

typedef enum Device_ {
  TIMER,
  KEYBOARD,
  MOUSE
} Device;

typedef enum GameStateMachine_{
  MAINMENU,
  PLAY,
  HELP,
  SETTINGS,
  PAUSE,
  EXIT
} GameStateMachine;

int (game_loop)();

void (interrupt_handler_multiplexer)(Device driver);

void (main_menu_interrupt_handler)(Device driver);

void (play_interrupt_handler)(Device driver);

void (help_interrupt_handler)(Device driver);

void (settings_interrupt_handler)(Device driver);

void (pause_menu_interrupt_handler)(Device driver);

#endif
