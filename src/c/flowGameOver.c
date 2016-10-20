#include <pebble.h>
#include "flowGameOver.h"

#include "main.h"
#include "bg.h"
#include "flowGame.h"
#include "key.h"
#include "screen.h"
#include "flow.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------


void flowGameOverSetup() {
  
}

void flowGameOverFinish() {
  
}

void flowGameOverUpdate() {
  if( keyIsTrigger( BUTTON_ID_SELECT ) ) {
    flowChange( FLOW_TITLE );
    return;
  }
}

void flowGameOverDraw( Layer *tgt, GContext *ctx ) {
  char msg[ 64 ];
  GRect box;
  //--- title
  box.origin.x = 0;
  box.origin.y = 44;
  box.size.w = 144;
  box.size.h = 32;
  snprintf( msg, sizeof(msg), "GAME\nOVER" );
  graphics_draw_text( ctx, msg, getFontTelop(), box, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL );
}

