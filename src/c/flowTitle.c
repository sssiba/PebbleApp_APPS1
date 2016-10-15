#include <pebble.h>
#include "flow.h"
#include "flowTitle.h"

#include "main.h"
#include "bg.h"
#include "player.h"
#include "flowGame.h"
#include "key.h"
#include "screen.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
#define TIME_PROMPT_SHOW 45
#define TIME_PROMPT_HIDE 15

#define PROMPTX_MIN TOFIX( -120 )
#define PROMPTX_MAX TOFIX( 20 )
#define PROMPTY TOFIX( 106 )
#define PROMPTSPD TOFIX( -1.25f )

static bool g_showprompt;
static uint16_t g_ivprompt;
static GPoint g_promptpos;

void flowTitleSetup() {
  bgInit();
  
  GRect* srect = scrGetRect();
  g_promptpos.x = TOFIX( srect->origin.x + srect->size.w) + PROMPTX_MAX;
  g_promptpos.y = PROMPTY;
}

void flowTitleFinish() {
  bgEnd();
}

void flowTitleUpdate() {
  if( keyIsTrigger( BUTTON_ID_SELECT ) ) {
    flowChange( FLOW_GAME );
    return;
  }
  
  GRect* srect = scrGetRect();
  g_promptpos.x += PROMPTSPD;
  if( g_promptpos.x <= PROMPTX_MIN ) {
    g_promptpos.x = TOFIX( srect->origin.x + srect->size.w) + PROMPTX_MAX;
  }
  
  bgUpdate();
}

void flowTitleDraw( Layer *tgt, GContext *ctx ) {
  bgDraw( tgt, ctx );
  
  char msg[ 64 ];
  GRect box;
  //--- title
  box.origin.x = 0;
  box.origin.y = 20;
  box.size.w = 144;
  box.size.h = 32;
  snprintf( msg, sizeof(msg), "Moon\nGurads" );
  graphics_draw_text( ctx, msg, getFontTelop(), box, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL );

  //--- time
  time_t temp = time(NULL);
  strftime( msg, sizeof(msg), clock_is_24h_style() ? "%H:%M:%S" : "%I:%M:%S", localtime( &temp ) );
  box.origin.x = 0;
  box.origin.y = 156;
  box.size.h = 12;
  graphics_draw_text( ctx, msg, getFont(), box, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL );
  
  //--- press select
  box.origin.x = TOINT( g_promptpos.x );
  box.origin.y = TOINT( g_promptpos.y );
  snprintf( msg, sizeof(msg), "PRESS SELECT" );
  graphics_draw_text( ctx, msg, getFont(), box, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL );

  
  graphics_context_set_stroke_color( ctx, GColorWhite );
  
  graphics_draw_line( ctx, GPoint( 0, GROUND_Y ), GPoint( 144, 120 ) );

}
