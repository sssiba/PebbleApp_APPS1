#include <pebble.h>
#include "main.h"
#include "objRock.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

void objRockSSetup( OBJDATA *od ) {
  od->state = 1;
  od->wait = 1;
}

void objRockSFinish( OBJDATA *od ) {
  
}

void objRockSUpdate( OBJDATA *od ) {
  if( --od->wait == 0 ) {
    if( ++od->state > 3 ) {
      objDel( od );
      return;
    }
    od->wait = 1;
  }
}

void objRockSDraw( Layer *tgt, GContext *ctx, OBJDATA *od ) {
  graphics_context_set_stroke_color( ctx, GColorWhite );
  graphics_draw_circle( ctx, GPoint( TOINT( od->pos.x ), TOINT( od->pos.y ) ), od->state );
}

//----------------------------------------------------------------
void createRockS( GPoint pos ) {
  OBJDATA *od = objAdd( OBJ_TYPE_ENEMY, objRockSSetup, objRockSFinish, objRockSUpdate, objRockSDraw );
  
  od->pos = pos;
}



//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

