#include <pebble.h>
#include "main.h"
#include "screen.h"
#include "player.h"
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

static const GPathInfo g_plpnt = {
  4, (GPoint []){
    {-6,  3},
    { 6,  3},
    { 6, -3},
    {-6, -3},
  }
};
static GPath *g_plpath;
static GPoint g_pos;

static GPoint g_acc;

static int16_t g_shotwait;

void plInit() {
  g_plpath = gpath_create( &g_plpnt );

  g_pos.x = TOFIX( 24 );
  g_pos.y = MAX_Y;
  g_acc.x = g_acc.y = 0;
  g_shotwait = 0;
}

void plEnd() {
  gpath_destroy( g_plpath );
}

#define RATE_DEC TOFIX( -1.0f )
#define RATE_ACC (-RATE_DEC + TOFIX(1.25f)) //(1.25f * 100))
#define MIN_RATE_ACC TOFIX(-2.0f) //(-2.0f * 100)
#define MAX_RATE_ACC TOFIX( 3.0f) //( 3.0f * 100)


void plUpdate() {
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
  g_pos.x += g_acc.x / 100;
#else
  //加速度固定の方がそれっぽい感じ
  if( keyIsPress( BUTTON_ID_UP ) ) {
    g_acc.x = TOFIX( 1.0f ); //1.0f * 100;
  } else {
    g_acc.x = TOFIX( -1.0f ); //-1.0f * 100;
  }
#endif
//  APPLOG( "%d(%d) %d(%d)", g_acc.x, g_acc.x / 100, g_acc.y, g_acc.y/100 );
  g_pos.x += g_acc.x; // / 100;
  g_pos.y += g_acc.y; // / 100;
  if( g_pos.x < MIN_X ) g_pos.x = MIN_X;
  if( g_pos.x > MAX_X ) g_pos.x = MAX_X;
  if( g_pos.y > MAX_Y ) {
    g_pos.y = MAX_Y;
    g_acc.y = 0; //着地
  }

  //自動弾発射
  if( ++g_shotwait >= SHOT_INTERVAL ) {
    pbShootV();
    pbShootH();
    g_shotwait = 0;
  }
}

void plDraw( Layer *tgt, GContext *ctx ) {
  GPoint p;
  p.x = TOINT( g_pos.x );
  p.y = TOINT( g_pos.y );
  gpath_move_to( g_plpath, p );
  gpath_rotate_to( g_plpath, 0 );
  
  graphics_context_set_stroke_color( ctx, GColorWhite );
  gpath_draw_outline( ctx, g_plpath );
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
GPoint plGetPos() {
  return g_pos;
}

void plSetPos( int x, int y ) {
  g_pos.x = x;
  g_pos.y = x;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#define MAX_PB_V 3 //上最大数
#define MAX_PB_H 3 //前最大数
#define PB_LEN 4 //弾のラインの長さ
#define PB_SPD TOFIX( 3 )

typedef struct {
  uint8_t life;
  GPoint pos; //弾の先端の位置
} PLBULLET;

static PLBULLET g_pbV[ MAX_PB_V ];
static PLBULLET g_pbH[ MAX_PB_H ];
static int16_t g_pbVcnt, g_pbHcnt;

void pbInit() {
  for( int i=0; i<MAX_PB_V; i++ ) {
    g_pbV[i].life = 0;
  }
  for( int i=0; i<MAX_PB_H; i++ ) {
    g_pbH[i].life = 0;
  }
  g_pbVcnt = g_pbHcnt = 0;
}

void pbEnd() {
  
}

void pbUpdate() {
  GRect* sr = scrGetRect();
  
  for( int i=0; i<MAX_PB_V; i++ ) {
    if( !g_pbV[i].life ) continue;
    
    g_pbV[i].pos.y -= PB_SPD;
    if( TOINT( g_pbV[i].pos.y ) <= sr->origin.y-PB_LEN ) {
      g_pbV[i].life = 1; //この直後消される
    }
    if( --g_pbV[i].life == 0 ) {
      g_pbVcnt--;
    }
  }
  
  for( int i=0; i<MAX_PB_H; i++ ) {
    if( !g_pbH[i].life ) continue;
    
    g_pbH[i].pos.x += PB_SPD;
    if( TOINT( g_pbH[i].pos.x ) >= ((sr->origin.x + sr->size.w) + PB_LEN) ) {
      g_pbH[i].life = 1; //この直後消される
    }
    if( --g_pbH[i].life == 0 ) {
      //横方向の弾が時間で消える場合はエフェクト出す
      createExpSmall( g_pbH[i].pos );
      g_pbHcnt--;
    }
  }
}

void pbDraw( Layer *tgt, GContext *ctx ) {
  graphics_context_set_stroke_color( ctx, GColorWhite );

  for( int i=0; i<MAX_PB_V; i++ ) {
    if( !g_pbV[i].life ) continue;
    GPoint ps, pe;
    ps.x = pe.x = TOINT( g_pbV[i].pos.x );
    ps.y = pe.y = TOINT( g_pbV[i].pos.y );
    pe.y += PB_LEN;
    
    graphics_draw_line( ctx, ps, pe );
  }
  
  for( int i=0; i<MAX_PB_H; i++ ) {
    if( !g_pbH[i].life ) continue;
    GPoint ps, pe;
    ps.x = pe.x = TOINT( g_pbH[i].pos.x );
    ps.y = pe.y = TOINT( g_pbH[i].pos.y );
    pe.x += PB_LEN;
    
    graphics_draw_line( ctx, ps, pe );
  }
  
}

void pbShootV()
{
  if( g_pbVcnt >= MAX_PB_V ) return;
  for( int i=0; i<MAX_PB_V; i++ ) {
    if( !g_pbV[i].life ) {
      GPoint pp = plGetPos();
      
      g_pbV[i].pos.x = pp.x;
      g_pbV[i].pos.y = pp.y - TOFIX( 4 + PB_LEN );

      g_pbV[i].life = 255; //垂直方向は時間で消えない
      g_pbVcnt++;
      break;
    }
  }
}

void pbShootH()
{
  if( g_pbHcnt >= MAX_PB_H ) return;  
  for( int i=0; i<MAX_PB_H; i++ ) {
    if( !g_pbH[i].life ) {
      GPoint pp = plGetPos();
      
      g_pbH[i].pos.x = pp.x + TOFIX( 5 + PB_LEN );
      g_pbH[i].pos.y = pp.y;

      g_pbH[i].life = 20;
      g_pbHcnt++;
      break;
    }
  }
}
