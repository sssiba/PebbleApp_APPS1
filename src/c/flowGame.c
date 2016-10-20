#include <pebble.h>
#include "main.h"
#include "bg.h"
#include "flowGame.h"
#include "flow.h"
#include "key.h"
#include "object.h"
#include "entry.h"
#include "drawAnim.h"
#include "objPlayer.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static uint8_t s_left;
static uint32_t s_score;

static OBJDATA *s_objplayer;

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

void flowGameSetup() {
  s_left = 2;
  s_score = 0;

  objInit();
  bgInit();
  entryInit();
  
  s_objplayer = objAdd( OBJ_TYPE_PLAYER, objPlayerSetup );
}

void flowGameFinish() {
  objDel( s_objplayer );
  
  entryEnd();
  bgEnd();
  objEnd();
}

void flowGameUpdate() {
  if( keyIsTrigger( BUTTON_ID_SELECT ) ) {
    flowChange( FLOW_GAMEOVER );
    return;
  }
  
  bgUpdate();
  objUpdate();
  objCollisionAll();
  
  entryUpdate();
}

void flowGameDraw( Layer *tgt, GContext *ctx ) {
//  APPLOG( "gamedraw" );
  
  bgDraw( tgt, ctx );
  
  objDraw( tgt, ctx );
  
  graphics_context_set_stroke_color( ctx, GColorWhite );
  
  graphics_draw_line( ctx, GPoint( 0, GROUND_Y ), GPoint( 144, 120 ) );
  
  
  char msg[ 64 ];
  GRect box;
  box.origin.x = 0;
  box.origin.y = 142;
  box.size.w = 144;
  box.size.h = 24;
  snprintf( msg, sizeof(msg), "LEFT:%d\nSCORE:%lu", s_left, s_score );
  graphics_draw_text( ctx, msg, getFont(), box, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL );
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void addScore( uint32_t v ) {
  s_score += v;
}

void addLeft( int8_t v ) {
  s_left += v;
}

uint32_t getScore() {
  return s_score;
}

uint8_t getLeft() {
  return s_left;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
