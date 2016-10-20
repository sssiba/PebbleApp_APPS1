#include <pebble.h>
#include "main.h"
#include "screen.h"
#include "object.h"
#include "objPlayer.h"
#include "key.h"

#include "objExplosion.h"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#define MIN_X TOFIX( 6 )
#define MAX_X TOFIX( 100 )
#define MAX_Y TOFIX(GROUND_Y - 4)

#if 0
#define JUMP_V0 TOFIX(-3.5f)
#define GRV_Y TOFIX(0.20f)
#else
#define JUMP_V0 TOFIX(-2.5f)
#define GRV_Y TOFIX(0.20f)
#endif

#define SHOT_INTERVAL 15
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

static GPath g_pathpl = {
  4, (GPoint []){
    {-6,  3},
    { 6,  3},
    { 6, -3},
    {-6, -3},
  },
  0,
  { 0, 0 },
};
static DDATA g_pldd = {
  DTYPE_STROKE,
  { TOFIX(0), TOFIX(0) }, TOFIX(0),
  {.argb = GColorWhiteARGB8 }, {.argb = GColorWhiteARGB8 }, 
  {.stroke.path = &g_pathpl },
};

static DDSET g_plddset = {
  1,
  (DDATA* []){ &g_pldd }
};


static GPoint g_acc;
static int16_t g_shotwait;
static OBJDATA *s_objplayer;

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------

void objPlayerSetup( OBJDATA *od ) {
  s_objplayer = od;
  
  od->funcFinish = objPlayerFinish;
  od->funcUpdate = objPlayerUpdate;
  od->funcDraw = objPlayerDraw;

  od->pos.x = TOFIX( 24 );
  od->pos.y = MAX_Y;
  g_acc.x = g_acc.y = 0;
  g_shotwait = 0;

  //objDel() 内で抹消
  od->drawanim = createDrawAnimWithDDSET( &g_plddset );

  //当たり判定設定
  od->cdata.type = COLTYPE_SQUARE;
  od->cdata.obj = od;
  od->cdata.info.square.rect.origin.x = TOINT( 0 );
  od->cdata.info.square.rect.origin.y = TOINT( 0 );
  od->cdata.info.square.rect.size.w = TOINT( 12 );
  od->cdata.info.square.rect.size.h = TOINT( 6 );
}

void objPlayerFinish( OBJDATA *od ) {
  s_objplayer = NULL;
}

#define RATE_DEC TOFIX( -1.0f )
#define RATE_ACC (-RATE_DEC + TOFIX(1.25f)) //(1.25f * 100))
#define MIN_RATE_ACC TOFIX(-2.0f) //(-2.0f * 100)
#define MAX_RATE_ACC TOFIX( 3.0f) //( 3.0f * 100)


void objPlayerUpdate( OBJDATA *od ) {
//  if( keyIsTrigger( BUTTON_ID_DOWN ) ) { //jump
  if( keyIsPress( BUTTON_ID_DOWN ) ) {
    if( g_acc.y == 0 ) {
      g_acc.y = JUMP_V0;
    }
  }
  g_acc.y += GRV_Y;
  
#if 0
  //基本常に減速、ボタン押してる間は加速
  g_acc.x += RATE_DEC;
  if( g_acc.x < MIN_RATE_ACC ) {
    g_acc.x = MIN_RATE_ACC;
  }
  if( keyIsPress( BUTTON_ID_UP ) ) { //forward
    g_acc.x += RATE_ACC;
    if( g_acc.x >= MAX_RATE_ACC ) {
      g_acc.x = MAX_RATE_ACC;
    }
  }
  od->pos.x += g_acc.x / 100;
#else
  //加速度固定の方がそれっぽい感じ
  if( keyIsPress( BUTTON_ID_UP ) ) {
    g_acc.x = TOFIX( 1.0f ); //1.0f * 100;
  } else {
    g_acc.x = TOFIX( -1.0f ); //-1.0f * 100;
  }
#endif
//  APPLOG( "%d(%d) %d(%d)", g_acc.x, g_acc.x / 100, g_acc.y, g_acc.y/100 );
  od->pos.x += g_acc.x; // / 100;
  od->pos.y += g_acc.y; // / 100;
  if( od->pos.x < MIN_X ) od->pos.x = MIN_X;
  if( od->pos.x > MAX_X ) od->pos.x = MAX_X;
  if( od->pos.y > MAX_Y ) {
    od->pos.y = MAX_Y;
    g_acc.y = 0; //着地
  }

  //自動弾発射
  if( ++g_shotwait >= SHOT_INTERVAL ) {
    pbShootV();
    pbShootH();
    g_shotwait = 0;
  }
}

void objPlayerDraw( Layer *tgt, GContext *ctx, OBJDATA *od ) {
  //drawAnimDraw( tgt, ctx, od->drawanim );
  
//  pbDraw( tgt, ctx );
}

//---------------------------------------------------------------------
GPoint plGetPos() {
  return s_objplayer->pos;
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#define MAX_PB_V 3 //上最大数
#define MAX_PB_H 3 //前最大数
#define PB_LEN 4 //弾のラインの長さ
#define PB_SPD TOFIX( 3 )

static int16_t s_pbVcnt, s_pbHcnt;


// 上方向弾発射
void pbShootV()
{
  if( s_pbVcnt >= MAX_PB_V ) return;
  s_pbVcnt++;
  GPoint pp = plGetPos();

  OBJDATA* od = objAdd( OBJ_TYPE_PLAYER, objPBVSetup );
  od->pos.x = pp.x;
  od->pos.y = pp.y - TOFIX( 4 + PB_LEN );
}

// 前方向弾発射
void pbShootH()
{
  if( s_pbHcnt >= MAX_PB_H ) return;  
  s_pbHcnt++;
  GPoint pp = plGetPos();
  
  OBJDATA* od = objAdd( OBJ_TYPE_PLAYER, objPBHSetup );
  od->pos.x = pp.x + TOFIX( 5 + PB_LEN );
  od->pos.y = pp.y;
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
static DDATA s_pbVdd = {
  DTYPE_LINE,
  { TOFIX(0), TOFIX(0) }, TOFIX(0),
  {.argb = GColorWhiteARGB8 }, {.argb = GColorWhiteARGB8 }, 
  {.line.pe = { 0,
                TOFIX(PB_LEN) }
  },
};
static DDSET s_pbVddset = {
  1,
  (DDATA* []){ &s_pbVdd }
};

static DDATA s_pbHdd = {
  DTYPE_LINE,
  { TOFIX(0), TOFIX(0) }, TOFIX(0),
  {.argb = GColorWhiteARGB8 }, {.argb = GColorWhiteARGB8 }, 
  {.line.pe = { TOFIX( -PB_LEN ),
                TOFIX( 0 ) }
  },
};
static DDSET s_pbHddset = {
  1,
  (DDATA* []){ &s_pbHdd }
};

#define PB_MX prm[0]
#define PB_MY prm[1]

void objPBVSetup( OBJDATA* od ) {
  od->funcFinish = objPBFinish;
  od->funcUpdate = objPBUpdate;
  od->funcHit = objPBHit;

  GPoint pp = plGetPos();
      
  od->pos.x = pp.x;
  od->pos.y = pp.y - TOFIX( 4 + PB_LEN );

  //objDel() 内で抹消
  od->drawanim = createDrawAnimWithDDSET( &s_pbVddset );
  
  od->PB_MX = TOFIX(0);
  od->PB_MY = -PB_SPD;
  
  od->wait = 255; //縦方向は時間で消えない

  //当たり判定設定
//  APPLOG( "V(%p) %p", od, &od->cdata );
  od->cdata.type = COLTYPE_POINT;
  od->cdata.obj = od;
}

void objPBHSetup( OBJDATA* od ) {
  od->funcFinish = objPBFinish;
  od->funcUpdate = objPBUpdate;
  od->funcHit = objPBHit;

  GPoint pp = plGetPos();
      
  od->pos.x = pp.x + TOFIX( 5 + PB_LEN );
  od->pos.y = pp.y;

  //objDel() 内で抹消
  od->drawanim = createDrawAnimWithDDSET( &s_pbHddset );

  od->PB_MX = PB_SPD;
  od->PB_MY = TOFIX(0);
  
  od->wait = 20; //一定時間後消滅

  //当たり判定設定
//  APPLOG( "H(%p) %p", od, &od->cdata );
  od->cdata.type = COLTYPE_POINT;
  od->cdata.obj = od;
}

void objPBFinish( OBJDATA* od ) {
    if( od->PB_MX != 0 ) {
      s_pbHcnt--;
    } else {
      s_pbVcnt--;
    }
}

void objPBUpdate( OBJDATA* od ) {
  GRect* sr = scrGetRect();

  od->pos.x += od->PB_MX;
  od->pos.y += od->PB_MY;
  
  if( TOINT( od->pos.y ) <= sr->origin.y - PB_LEN ) {
    od->wait = 1; //この直後消される
  } else
  if( TOINT( od->pos.x ) >= ((sr->origin.x + sr->size.w) + PB_LEN) ) {
    od->wait = 1; //この直後消される
  }
  
  if( --od->wait <= 0 ) {
    //横方向の弾が消える場合はエフェクト
    if( od->PB_MX != 0 ) {
      createExpSmall( od->pos );
    }
    objDel( od );
    return;
  }
}

void objPBHit( OBJDATA *od, OBJDATA *tgt )
{
  createExpSmall( od->pos );
  objDel( od );
}
