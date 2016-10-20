#include <pebble.h>
#include "main.h"
#include "objRock.h"
#include "flowGame.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static GPath s_pathS = {
  6,
  (GPoint []){
    { 0,   0},
    { 2,  -4},
    { 4,  -7},
    { 9,  -5},
    {12,  -4},
    {14,   0},
  },
  0,
  { 0, 0 },
};
static DDATA s_ddS = {
  DTYPE_STROKE_OPEN,
  { TOFIX(0), TOFIX(0) }, TOFIX(0),
  {.argb = GColorWhiteARGB8 }, {.argb = GColorWhiteARGB8 }, 
  {.stroke.path = &s_pathS },
};

static DDSET s_ddsetS = {
  1,
  (DDATA* []){ &s_ddS }
};


void objRockSSetup( OBJDATA *od ) {
  od->funcFinish = objRockSFinish;
  od->funcUpdate = objRockSUpdate;
//  od->funcDraw = objRockSDraw;
  od->funcHit = objRockSHit;

  od->state = 1;
  od->wait = 1;

  //objDel() 内で抹消
  od->drawanim = createDrawAnimWithDDSET( &s_ddsetS );
  
  
  //当たり判定設定
  od->cdata.type = COLTYPE_SQUARE;
  od->cdata.obj = od;
  od->cdata.info.square.rect.origin.x = TOFIX(0);
  od->cdata.info.square.rect.origin.y = TOFIX(-6);
  od->cdata.info.square.rect.size.w = TOFIX(14);
  od->cdata.info.square.rect.size.h = TOFIX(6);
}

void objRockSFinish( OBJDATA *od ) {
}

void objRockSUpdate( OBJDATA *od ) {
  od->pos.x += GROUND_SPD;
  
  if( od->pos.x < TOFIX( -15 ) ) {
    objDel( od );
    return;
  }
}

void objRockSHit( OBJDATA *od, OBJDATA *tgt ) {
  objDel( od );
  
  addScore( 10 );
}

//----------------------------------------------------------------
void createRockS( GPoint pos ) {
  OBJDATA *od = objAdd( OBJ_TYPE_ENEMY, objRockSSetup );
  
  od->pos = pos;
}



//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

