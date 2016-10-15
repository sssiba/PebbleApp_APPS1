#include <pebble.h>
#include "main.h"
#include "bg.h"
#include "player.h"
#include "flowGame.h"
#include "flow.h"
#include "key.h"
#include "object.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static uint8_t left;
static uint32_t score;

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

void flowGameSetup() {
  left = 2;
  score = 0;
  
  objInit();
  plInit();
  pbInit();
  bgInit();
}

void flowGameFinish() {
  bgEnd();
  pbEnd();
  plEnd();
  objEnd();
}

void flowGameUpdate() {
  if( keyIsTrigger( BUTTON_ID_SELECT ) ) {
    flowChange( FLOW_GAMEOVER );
    return;
  }
  
  objUpdate();
  
  bgUpdate();
  plUpdate();
  pbUpdate();
}

void flowGameDraw( Layer *tgt, GContext *ctx ) {
//  APPLOG( "gamedraw" );
  
  bgDraw( tgt, ctx );
  
  objDraw( tgt, ctx );
  
  plDraw( tgt, ctx );
  pbDraw( tgt, ctx );
  
  graphics_context_set_stroke_color( ctx, GColorWhite );
  
  graphics_draw_line( ctx, GPoint( 0, GROUND_Y ), GPoint( 144, 120 ) );
  
  
  char msg[ 64 ];
  GRect box;
  box.origin.x = 0;
  box.origin.y = 142;
  box.size.w = 144;
  box.size.h = 24;
  snprintf( msg, sizeof(msg), "LEFT:%d\nSCORE:%lu", left, score );
  graphics_draw_text( ctx, msg, getFont(), box, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL );
  
}
