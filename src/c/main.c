#include <pebble.h>
#include "main.h"
#include "screen.h"
#include "bg.h"
#include "player.h"
#include "key.h"
#include "flow.h"

static Window *my_window;

static uint32_t g_frame;

static GFont g_font, g_fonttelop;

#define UPDATE_INTERVAL 32

//---------------------------------------------
static void appTimerCB( void *data) 
{
  //状態更新
  keyUpdate();
  flowUpdate();
  
  //画面再描画設定(いる？)
  scrDirty();

  ++g_frame;
  
  //再設定必要
  int t = UPDATE_INTERVAL; //x!x! 今回かかった時間を引くべき
  app_timer_register( t, appTimerCB, NULL );
}

uint32_t getFrame() {
  return g_frame;
}

GFont getFont() {
  return g_font;
}

GFont getFontTelop() {
  return g_fonttelop;
}

//---------------------------------------------

void handle_init(void) {
  my_window = window_create();
  
  g_font = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_NORMAL_12 ) );
  g_fonttelop = fonts_load_custom_font( resource_get_handle( RESOURCE_ID_FONT_TELOP_20 ) );
  

  g_frame = 0;
  
  keyInit( my_window );
  scrInit( my_window );
  
  flowInit();
  flowChange( FLOW_TITLE );
  
  window_stack_push(my_window, true);

  
  // timer
  app_timer_register( UPDATE_INTERVAL, appTimerCB, NULL );
  
}

void handle_deinit(void) {
  window_destroy(my_window);

  flowEnd();
  scrEnd();
  keyEnd();
  
  fonts_unload_custom_font( g_font );
  fonts_unload_custom_font( g_fonttelop );
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
