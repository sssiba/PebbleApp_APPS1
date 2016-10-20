#include <pebble.h>
#include "main.h"
#include "bg.h"
#include "screen.h"

#if 0
#define STAR_REGION 0.33f //上画面 1/3
#define MT_REGION 0.20f   //星の下
#else
#define STAR_REGION 0.25f //上画面 1/4
#define MT_REGION 0.175f   //星の下
#define NEAR_REGION 0.10f //遠景の下
#endif

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

static void starInit();
static void starEnd();
static void starDraw( Layer *tgt, GContext *ctx );
static void starUpdate();

//-----------------------------------------------------------------------

static void mountainInit();
static void mountainEnd();
static void mountainDraw( Layer *tgt, GContext *ctx );
static void mountainUpdate();

//-----------------------------------------------------------------------

static void nearInit();
static void nearEnd();
static void nearDraw( Layer *tgt, GContext *ctx );
static void nearUpdate();


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

void bgInit() {
  starInit();
  mountainInit();
  nearInit();
}

void bgEnd() {
  starEnd();
  mountainEnd();
  nearEnd();
}

void bgDraw( Layer *tgt, GContext *ctx ) {
  starDraw( tgt, ctx );
  mountainDraw( tgt, ctx );
  nearDraw( tgt, ctx );
}

void bgUpdate() {
  starUpdate();
  mountainUpdate();
  nearUpdate();
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define MAX_STAR 20
#define ADD_RATE (0.30f*100)
#define TIME_MOVE 30

static GRect g_starregion;
static GPoint g_starpos[ MAX_STAR ];

static int g_starcnt;
static int g_movewait;
static int g_emptyidx;

void starInit() {
  GRect* srect = scrGetRect();

  //星の出現領域
  g_starregion = *srect;
  g_starregion.size.h = srect->size.h * STAR_REGION;
  
  for( int i=0; i<MAX_STAR/3*2; i++ ) {
    g_starpos[i].x = g_starregion.origin.x + rand() % g_starregion.size.w;
    g_starpos[i].y = g_starregion.origin.y + rand() % g_starregion.size.h;
  }
  
  g_starcnt = MAX_STAR/3*2;
  g_movewait = 0;
  g_emptyidx = -1;
}

void starEnd() {
  
}

void starUpdate() {
  if( ++g_movewait < TIME_MOVE ) return;
  
  int emptyidx = -1;
  
  //移動
  for( int i=0; i<MAX_STAR; i++ ) {
    if( --g_starpos[i].x < 0 ) {
      emptyidx = i;
    }
  }
  
  //新規星追加
  if( (rand() % 100) < ADD_RATE ) {
    //空いてる場所があれば追加
    if( emptyidx >= 0 ) {
      g_starpos[emptyidx].x = g_starregion.origin.x + g_starregion.size.w; //右端
      g_starpos[emptyidx].y = g_starregion.origin.y + rand() % g_starregion.size.h;
    }    
  }
  
  g_movewait = 0;
}

void starDraw( Layer *tgt, GContext *ctx ) {
  graphics_context_set_stroke_color( ctx, GColorWhite );

  for( int i=0; i<MAX_STAR; i++ ) {
    graphics_draw_pixel( ctx, g_starpos[i] );
  }
  
}

//-----------------------------------------------------------------------
#define MAX_MTVTX 16
#define MIN_MTLEN 10
#define MAX_MTLEN 25
#define TIME_MTMOVE 12


static GPoint g_mtvtx[ MAX_MTVTX ];
static GRect g_mtregion;
static int g_mtread;
static GPath *g_mtpath;
static GPathInfo g_mtpathinfo;
static int g_mtmovewait;

void mountainInit() {
  GRect* srect = scrGetRect();

  //星の出現領域
  g_mtregion = *srect;
  g_mtregion.origin.y = g_starregion.origin.y + g_starregion.size.h;
  g_mtregion.size.h = srect->size.h * MT_REGION;
  
  g_mtvtx[0].x = -MIN_MTLEN;
  g_mtvtx[0].y = g_mtregion.origin.y + rand() % g_mtregion.size.h;
  for( int i=1; i<MAX_MTVTX; i++ ) {
    g_mtvtx[i].x = g_mtvtx[i-1].x + (MIN_MTLEN + rand() % MAX_MTLEN);
    g_mtvtx[i].y = g_mtregion.origin.y + rand() % g_mtregion.size.h;
  }
  g_mtread = 0;

  g_mtpathinfo.num_points = MAX_MTVTX;
  g_mtpathinfo.points = malloc( sizeof( GPoint ) * MAX_MTVTX );
  
  g_mtmovewait = 0;
}

void mountainEnd() {
  if( g_mtpath ) {
    gpath_destroy( g_mtpath );
    g_mtpath = NULL;
  }
  if( g_mtpathinfo.points ) {
    free( g_mtpathinfo.points );
    g_mtpathinfo.points = NULL;
  }
}

void mountainUpdate() {
  
  if( ++g_mtmovewait < TIME_MTMOVE ) return;
  
  for( int i=0; i<MAX_MTVTX; i++ ) {
    --g_mtvtx[i].x;
  }

  //画面外に頂点が出たら消して、末尾に追加
  //ラインの関係上、左端の２頂点が出た時点で左端のみ削除
  int i0 = g_mtread;
  int i1 = g_mtread + 1;
  if( i1 >= MAX_MTVTX ) i1 -= MAX_MTVTX;
  
  if( (g_mtvtx[i0].x < 0) && (g_mtvtx[i1].x < 0) ) {
    g_mtread = i1;
    int last = i0 + (MAX_MTVTX-1);
    if( last >= MAX_MTVTX ) last -= MAX_MTVTX;
    g_mtvtx[i0].x = g_mtvtx[last].x + (MIN_MTLEN + rand() % MAX_MTLEN);
    g_mtvtx[i0].y = g_mtregion.origin.y + rand() % g_mtregion.size.h;
  }
  
  g_mtmovewait = 0;
}

void mountainDraw( Layer *tgt, GContext *ctx ) {
  int idx = g_mtread;
  for( int i=0; i<MAX_MTVTX; i++ ) {
    g_mtpathinfo.points[i] = g_mtvtx[idx];
    if( ++idx >= MAX_MTVTX ) idx -= MAX_MTVTX;
  }
  if( g_mtpath ) {
    gpath_destroy( g_mtpath );
  }
  g_mtpath = gpath_create( &g_mtpathinfo );
  gpath_move_to( g_mtpath, GPoint( 0, 0 ) );
  gpath_rotate_to( g_mtpath, 0 );
  
  graphics_context_set_stroke_color( ctx, GColorWhite );
  gpath_draw_outline_open( ctx, g_mtpath );
}

//-----------------------------------------------------------------------
#define MAX_NEARVTX 10
#define MIN_NEARLEN 25
#define MAX_NEARLEN 25
#define TIME_NEARMOVE 3


static GPoint g_nearvtx[ MAX_NEARVTX ];
static GRect g_nearregion;
static int g_nearread;
static GPath *g_nearpath;
static GPathInfo g_nearpathinfo;
static int g_nearmovewait;

void nearInit() {
  GRect* srect = scrGetRect();

  //星の出現領域
  g_nearregion = *srect;
  g_nearregion.origin.y = g_mtregion.origin.y + g_mtregion.size.h;
  g_nearregion.size.h = srect->size.h * NEAR_REGION;
  
  g_nearvtx[0].x = -MIN_NEARLEN;
  g_nearvtx[0].y = g_nearregion.origin.y + rand() % g_nearregion.size.h;
  for( int i=1; i<MAX_NEARVTX; i++ ) {
    g_nearvtx[i].x = g_nearvtx[i-1].x + (MIN_NEARLEN + rand() % MAX_NEARLEN);
//    g_nearvtx[i].y = g_nearregion.origin.y + rand() % g_nearregion.size.h;
      int gap = g_nearregion.size.h / 3;
      g_nearvtx[i].y = g_nearregion.origin.y + rand() % (g_nearregion.size.h - gap) + gap;
  }
  g_nearread = 0;

  g_nearpathinfo.num_points = MAX_NEARVTX;
  g_nearpathinfo.points = malloc( sizeof( GPoint ) * MAX_NEARVTX );
  
  g_nearmovewait = 0;
}

void nearEnd() {
  if( g_nearpath ) {
    gpath_destroy( g_nearpath );
    g_nearpath = NULL;
  }
  if( g_nearpathinfo.points ) {
    free( g_nearpathinfo.points );
    g_nearpathinfo.points = NULL;
  }
}

void nearUpdate() {
  
  if( ++g_nearmovewait < TIME_NEARMOVE ) return;
  
  for( int i=0; i<MAX_NEARVTX; i++ ) {
    --g_nearvtx[i].x;
  }

  //画面外に頂点が出たら消して、末尾に追加
  //ラインの関係上、左端の２頂点が出た時点で左端のみ削除
  int i0 = g_nearread;
  int i1 = g_nearread + 1;
  if( i1 >= MAX_NEARVTX ) i1 -= MAX_NEARVTX;
  
  if( (g_nearvtx[i0].x < 0) && (g_nearvtx[i1].x < 0) ) {
    g_nearread = i1;
    int last = i0 + (MAX_NEARVTX-1);
    if( last >= MAX_NEARVTX ) last -= MAX_NEARVTX;
    g_nearvtx[i0].x = g_nearvtx[last].x + (MIN_NEARLEN + rand() % MAX_NEARLEN);
//    g_nearvtx[i0].y = g_nearregion.origin.y + rand() % g_nearregion.size.h;
    int gap = g_nearregion.size.h / 3;
    g_nearvtx[i0].y = g_nearregion.origin.y + rand() % (g_nearregion.size.h - gap) + gap;
  }
  
  g_nearmovewait = 0;
}

void nearDraw( Layer *tgt, GContext *ctx ) {
  int idx = g_nearread;
  for( int i=0; i<MAX_NEARVTX; i++ ) {
    g_nearpathinfo.points[i] = g_nearvtx[idx];
    if( ++idx >= MAX_NEARVTX ) idx -= MAX_NEARVTX;
  }
  if( g_nearpath ) {
    gpath_destroy( g_nearpath );
  }
  g_nearpath = gpath_create( &g_nearpathinfo );
  gpath_move_to( g_nearpath, GPoint( 0, 0 ) );
  gpath_rotate_to( g_nearpath, 0 );
  
  graphics_context_set_stroke_color( ctx, GColorWhite );
  gpath_draw_outline_open( ctx, g_nearpath );
}
