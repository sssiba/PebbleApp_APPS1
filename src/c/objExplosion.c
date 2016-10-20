#include <pebble.h>
#include "main.h"
#include "objExplosion.h"


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
void objExpSetup( OBJDATA *od ) {
  od->funcFinish = objExpFinish;
  od->funcUpdate = objExpUpdate;
  od->funcDraw = objExpDraw;
  
  od->state = 1;
  od->wait = 1;
}

void objExpFinish( OBJDATA *od ) {
  
}

void objExpUpdate( OBJDATA *od ) {
  if( --od->wait == 0 ) {
    if( ++od->state > 3 ) {
      objDel( od );
      return;
    }
    od->wait = 1;
  }
}

void objExpDraw( Layer *tgt, GContext *ctx, OBJDATA *od ) {
  graphics_context_set_stroke_color( ctx, GColorWhite );
  graphics_draw_circle( ctx, GPoint( TOINT( od->pos.x ), TOINT( od->pos.y ) ), od->state );
}

//----------------------------------------------------------------
void createExpSmall( GPoint pos ) {
  OBJDATA *od = objAdd( OBJ_TYPE_EFFECT, objExpSetup );
  
  od->pos = pos;
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
