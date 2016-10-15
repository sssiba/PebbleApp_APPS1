#include <pebble.h>
#include "main.h"
#include "key.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
/*
BUTTON_ID_BACK
Back button.
BUTTON_ID_UP
Up button.
BUTTON_ID_SELECT
Select (middle) button.
BUTTON_ID_DOWN
Down button.
NUM_BUTTONS
Total number of buttons.
*/
static uint8_t g_keystate;  //ハンドラにより直ぐに反映される値
static uint8_t g_prvkeystate;
static uint8_t g_keytrg;
static uint8_t g_keypress;  //今回のフレームでの値

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static void keyUpPressCB(ClickRecognizerRef recognizer, void *context) {
  g_keystate |= 1 << BUTTON_ID_UP;
}
static void keyUpReleaseCB(ClickRecognizerRef recognizer, void *context) {
  g_keystate &= ~(1 << BUTTON_ID_UP );
}
static void keySelectPressCB(ClickRecognizerRef recognizer, void *context) {
  g_keystate |= 1 << BUTTON_ID_SELECT;
}
static void keySelectReleaseCB(ClickRecognizerRef recognizer, void *context) {
  g_keystate &= ~(1 << BUTTON_ID_SELECT );
}
static void keyDownPressCB(ClickRecognizerRef recognizer, void *context) {
  g_keystate |= 1 << BUTTON_ID_DOWN;
}
static void keyDownReleaseCB(ClickRecognizerRef recognizer, void *context) {
  g_keystate &= ~(1 << BUTTON_ID_DOWN );
}

static void keyClickConfigProvider( void * ctx ) {
  window_raw_click_subscribe( BUTTON_ID_UP, keyUpPressCB, keyUpReleaseCB, NULL );
  window_raw_click_subscribe( BUTTON_ID_SELECT, keySelectPressCB, keySelectReleaseCB, NULL );
  window_raw_click_subscribe( BUTTON_ID_DOWN, keyDownPressCB, keyDownReleaseCB, NULL );
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
void keyInit( Window* win ) {
  g_prvkeystate = g_keystate = g_keytrg = g_keypress = 0;
  window_set_click_config_provider( win, keyClickConfigProvider );
}

void keyEnd() {
  
}

void keyUpdate() {
  g_keypress = g_keystate;
  g_keytrg = g_keystate & (g_keystate ^ g_prvkeystate);
  
  g_prvkeystate = g_keystate;
}

//----------------------------------------------------------------
bool keyIsPress( ButtonId id ) {
  return g_keypress & (1 << id);
}

bool keyIsTrigger( ButtonId id ) {
  return g_keytrg & (1 << id);
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
