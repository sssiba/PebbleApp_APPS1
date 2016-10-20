#include <pebble.h>

#include "main.h"

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
  od->wait = 8 + rand() % 20;

  //objDel() 内で抹消
  od->drawanim = createDrawAnimWithDDSET( &s_ddset );
  
  
  //当たり判定設定
  od->cdata.type = COLTYPE_SQUARE;
  od->cdata.obj = od;
  od->cdata.info.square.rect.origin.x = TOFIX(-8);
  od->cdata.info.square.rect.origin.y = TOFIX(-4);
  od->cdata.info.square.rect.size.w = TOFIX(16);
  od->cdata.info.square.rect.size.h = TOFIX( 8);
}

void objEne1Finish( OBJDATA *od ) {
  
}

#define MVSPD TOFIX( 4.0f )
#define MVYRANGE 3
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
        od->prm[1] = rand() % 360;
        od->prm[2] = od->pos.y;
        od->state = 1;
      }
      break;
    case 1:
      {
        od->pos.x += od->prm[0];
        
        /*
  int32_t mang = TRIG_MAX_ANGLE * t->tm_min / 60;
  GPoint mhand = {
    .x = (int16_t)(sin_lookup(mang) * (int32_t)mhlen / TRIG_MAX_RATIO) + center.x,
    .y = (int16_t)(-cos_lookup(mang) * (int32_t)mhlen / TRIG_MAX_RATIO) + center.y,
  };
  */
        int32_t ang = TRIG_MAX_ANGLE * od->prm[1] / 360;
        int16_t my = (int16_t)(sin_lookup(ang) * MVYRANGE / TRIG_MAX_RATIO );
        
//        APPLOG( "ang:%lx(%ld)[%d]  slup:%lx(%ld)", ang, ang, DEG_TO_TRIGANGLE(od->prm[1]), sin_lookup(ang), sin_lookup(ang) );
        
        ang = od->prm[1];
        APPLOG( "ang:%d  sin:%d[%d]", od->prm[1], TOFIXSIN(ang, MVYRANGE ), TOINT( TOFIXSIN( ang, MVYRANGE ) ) );

        od->pos.y = od->prm[2] + TOFIX(my);
        
        if( ++od->prm[1] >= 360 ) od->prm[1] = 0;
        
        if( od->pos.x < TOFIX( 8 ) ) {
          od->pos.x = TOFIX(8);
          od->prm[0] = MVSPD;
        } else
        if( od->pos.x > TOFIX(136) ) {
          od->pos.x = TOFIX(136);
          od->prm[0] = -MVSPD;
        }
            
      }
      break;
  }
  
  
  
}

void objEne1Hit( OBJDATA *od, OBJDATA *tgt ) {
  
}

//----------------------------------------------------------------
void createEne1( GPoint pos ) {
  OBJDATA *od = objAdd( OBJ_TYPE_ENEMY, objEne1Setup );
  
  od->pos = pos;
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

