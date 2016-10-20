#include <pebble.h>

#include "main.h"
#include "entry.h"
#include "flowGame.h"

#include "objEnemy1.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static GPath s_path = {
  8,
  (GPoint []){
    { -8, -2},
    { -2, -4},
    {  2, -4},
    {  8, -2},
    {  8,  2},
    {  2,  4},
    { -2,  4},
    { -8,  2},
  },
  0,
  { 0, 0 },
};
static DDATA s_dd = {
  DTYPE_STROKE,
  { TOFIX(0), TOFIX(0) }, TOFIX(0),
  {.argb = GColorWhiteARGB8 }, {.argb = GColorWhiteARGB8 }, 
  {.stroke.path = &s_path },
};

static DDSET s_ddset = {
  1,
  (DDATA* []){ &s_dd }
};



//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
void objEne1Setup( OBJDATA *od ) {
  od->funcFinish = objEne1Finish;
  od->funcUpdate = objEne1Update;
//  od->funcDraw = objRockSDraw;
  od->funcHit = objEne1Hit;

  od->state = 0;
  od->wait = 14 + rand() % 20;
  od->cnt = 60 + rand() % 240; //滞在時間

  //objDel() 内で抹消
  od->drawanim = createDrawAnimWithDDSET( &s_ddset );
  
  
  //当たり判定設定
  od->cdata.type = COLTYPE_SQUARE;
  od->cdata.obj = od;
  od->cdata.info.square.rect.origin.x = TOFIX(-8);
  od->cdata.info.square.rect.origin.y = TOFIX(-4);
  od->cdata.info.square.rect.size.w = TOFIX(16);
  od->cdata.info.square.rect.size.h = TOFIX( 8);
  
  entryIncCount();
}

void objEne1Finish( OBJDATA *od ) {
  entryDecCount();
}

#define MVSPD TOFIX( 3.0f )
#define MVYRANGE 8
void objEne1Update( OBJDATA *od ) {
  switch( od->state ) {
    case 0:
      od->pos.y += TOFIX( 1.0f );
      if( --od->wait == 0 ) {
        if( rand() & 1 ) {
          od->prm[0] = MVSPD;
        } else {
          od->prm[0] = -MVSPD;
        }
        od->prm[1] = 0;
        od->prm[2] = od->pos.y;
        od->state = 1;
      }
      break;
    case 1:
      {
        od->pos.x += od->prm[0];
        od->pos.y = od->prm[2] + TOFIXSIN( od->prm[1], MVYRANGE );
        
        od->prm[1] += 6;
        
        if( od->prm[1] >= 360 ) od->prm[1] -= 360;
        
        if( od->cnt > 0 ) {
          od->cnt--;
          if( od->pos.x < TOFIX( 8 ) ) {
            od->pos.x = TOFIX(8);
            od->prm[0] = MVSPD;
          } else
          if( od->pos.x > TOFIX(136) ) {
            od->pos.x = TOFIX(136);
            od->prm[0] = -MVSPD;
          }
        } else {
          if( od->pos.x < TOFIX( -8 ) ||
              od->pos.x > TOFIX( 144 + 8 ) ) {
            //滞在時間過ぎたから画面外で消滅
            objDel( od );
            return;
          }
        }
      }
      break;
  }
}

void objEne1Hit( OBJDATA *od, OBJDATA *tgt ) {
  objDel( od );
  
  addScore( 50 );
}

//----------------------------------------------------------------
void createEne1( GPoint pos ) {
  OBJDATA *od = objAdd( OBJ_TYPE_ENEMY, objEne1Setup );
  
  od->pos = pos;
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

