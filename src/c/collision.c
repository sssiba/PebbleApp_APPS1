#include <pebble.h>
#include "main.h"
#include "collision.h"
#include "object.h"

//http://marupeke296.com/COL_main.html

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

static bool srcPoint( CDATA *src, CDATA *dst );
static bool srcSquare( CDATA *src, CDATA *dst );
static bool srcLine( CDATA *src, CDATA *dst );
static bool srcCircle( CDATA *src, CDATA *dst );

static bool chkPointPoint( CDATA *src, CDATA *dst ); //point x point
static bool chkPointSquare( CDATA *src, CDATA *dst ); //point x square
static bool chkPointLine( CDATA *src, CDATA *dst ); //point x line
static bool chkPointCircle( CDATA *src, CDATA *dst ); //point x circle

static bool chkSquareSquare( CDATA *src, CDATA *dst ); //square x square
static bool chkSquareLine( CDATA *src, CDATA *dst ); //square x line
static bool chkSquareCircle( CDATA *src, CDATA *dst ); //square circle

static bool chkLineLine( CDATA *src, CDATA *dst ); //line x line
static bool chkLineCircle( CDATA *src, CDATA *dst ); //line x circle

static bool chkCircleCircle( CDATA *src, CDATA *dst ); //circle x circle

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
bool colCheck( CDATA *src, CDATA *dst ) {
  bool ret = false;
  
  switch( src->type ) {
    case COLTYPE_POINT:
      ret = srcPoint( src, dst );
      break;
    case COLTYPE_SQUARE:
      ret = srcSquare( src, dst );
      break;
    case COLTYPE_LINE:
      ret = srcLine( src, dst );
      break;
    case COLTYPE_CIRCLE:
      ret = srcCircle( src, dst );
      break;
  }
  
  return ret;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
bool srcPoint( CDATA *src, CDATA *dst ) {
  bool ret = false;
  
  switch( dst->type ) {
    case COLTYPE_POINT:
      ret = chkPointPoint( src, dst );
      break;
    case COLTYPE_SQUARE:
      ret = chkPointSquare( src, dst );
      break;
    case COLTYPE_LINE:
      ret = chkPointLine( src, dst );
      break;
    case COLTYPE_CIRCLE:
      ret = chkPointCircle( src, dst );
      break;
  }
  
  return ret;
}

//----------------------------------------------------------------
bool srcSquare( CDATA *src, CDATA *dst ) {
  bool ret = false;
  
  switch( dst->type ) {
    case COLTYPE_POINT:
      ret = chkPointSquare( dst, src );
      break;
    case COLTYPE_SQUARE:
      ret = chkSquareSquare( src, dst );
      break;
    case COLTYPE_LINE:
      ret = chkSquareLine( src, dst );
      break;
    case COLTYPE_CIRCLE:
      ret = chkSquareCircle( src, dst );
      break;
  }
  
  return ret;
}

//----------------------------------------------------------------
bool srcLine( CDATA *src, CDATA *dst ) {
  bool ret = false;
  
  switch( dst->type ) {
    case COLTYPE_POINT:
      ret = chkPointLine( dst, src );
      break;
    case COLTYPE_SQUARE:
      ret = chkSquareLine( dst, src );
      break;
    case COLTYPE_LINE:
      ret = chkLineLine( src, dst );
      break;
    case COLTYPE_CIRCLE:
      ret = chkLineCircle( src, dst );
      break;
  }
  
  return ret;
}

//----------------------------------------------------------------
bool srcCircle( CDATA *src, CDATA *dst ) {
  bool ret = false;
  
  switch( dst->type ) {
    case COLTYPE_POINT:
      ret = chkPointCircle( dst, src );
      break;
    case COLTYPE_SQUARE:
      ret = chkSquareCircle( dst, src );
      break;
    case COLTYPE_LINE:
      ret = chkLineCircle( dst, src );
      break;
    case COLTYPE_CIRCLE:
      ret = chkCircleCircle( src, dst );
      break;
  }
  
  return ret;
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
//src:point dst:point
static bool chkPointPoint( CDATA *src, CDATA *dst ) {
  GPoint ps, pd;
  ps = toIntPos( src->obj->pos );
  pd = toIntPos( dst->obj->pos );
  
  if( ps.x == pd.x && ps.y == pd.y ) return true;
  
  return false;
}

//src:point dst:square
static bool chkPointSquare( CDATA *src, CDATA *dst ) {
  GPoint ps, pd0, pd1;
  GRect *r = &dst->info.square.rect;
  
//  APPLOG( "chkPS %p  s(%p):%p  d(%p):%p", r, src, src->obj, dst, dst->obj );

  ps = toIntPos( src->obj->pos );
  
  pd0 = dst->obj->pos;
  pd0.x += r->origin.x;
  pd0.y += r->origin.y;
  pd1 = pd0;
  pd1.x += r->size.w;
  pd1.y += r->size.h;

  pd0 = toIntPos( pd0 );
  pd1 = toIntPos( pd1 );
  /*
  APPLOG( "ps(%d,%d)  (%d,%d)-(%d,%d)", ps.x, ps.y,
         pd0.x, pd0.y, pd1.x, pd1.y
        );
  */
  if( pd0.x > pd1.x ) {
    int16_t t = pd1.x;
    pd1.x = pd0.x;
    pd0.x = t;
  }
  if( pd0.y > pd1.y ) {
    int16_t t = pd1.y;
    pd1.y = pd0.y;
    pd0.y = t;
  }
  if( (ps.x >= pd0.x) && (ps.y >= pd0.y) &&
      (ps.x <= pd1.x) && (ps.y <= pd1.y) ) {
    return true;
  }
  
  return false;
}

//src:point dst:line
static bool chkPointLine( CDATA *src, CDATA *dst ) {
  /*

線分の長さ L1 = sqrt( (x1-x0)^2 + (y1-y0)^2 )
線分の始点から点までの長さ L2 = sqrt( (x2-x0)^2 + (y2-y0)^2 )

(x1-x0)*(x2-x0) + (y1-y0)*(y2-y0) が L1*L2 に等しく、かつL1≧L2の時衝突している
*/
  return false;
}

//src:point dst:circle
static bool chkPointCircle( CDATA *src, CDATA *dst ) {
/*
円1 ： 中心点C1(xc1, yc1)、半径r1
　円2 ： 中心点C2(xc2, yc2)、半径r2

(xc1-xc2)^2 + (yc1-yc2)^2 ≦ (r1+r2)^2ならば衝突している
*/
  return false;
}

//src:square dst:square
static bool chkSquareSquare( CDATA *src, CDATA *dst ) {
  
  //左上原点の四角を中心原点の四角にする
  GRect sr;
  sr.size.w = TOINT( src->info.square.rect.size.w / 2 );
  sr.size.h = TOINT( src->info.square.rect.size.h / 2 );
  sr.origin.x = TOINT( src->info.square.rect.origin.x + src->obj->pos.x ) + sr.size.w;
  sr.origin.y = TOINT( src->info.square.rect.origin.y + src->obj->pos.y ) + sr.size.h;
  GRect dr;
  dr.size.w = TOINT( dst->info.square.rect.size.w / 2 );
  dr.size.h = TOINT( dst->info.square.rect.size.h / 2 );
  dr.origin.x = TOINT( dst->info.square.rect.origin.x + dst->obj->pos.x ) + dr.size.w;
  dr.origin.y = TOINT( dst->info.square.rect.origin.y + dst->obj->pos.y ) + dr.size.h;
  
  if( (abs( sr.origin.x - dr.origin.y ) < (sr.size.w + dr.size.w)) &&
      (abs( sr.origin.y - dr.origin.y ) < (sr.size.h + dr.size.h)) ) {
    return true;
  }
  
  return false;
}

//src:square dst:line
static bool chkSquareLine( CDATA *src, CDATA *dst ) {
  return false;  
}

//src:square dst:circle
static bool chkSquareCircle( CDATA *src, CDATA *dst ) {
  return false;  
}

//src:line dst:line
static bool chkLineLine( CDATA *src, CDATA *dst ) {
  /*

線分衝突判定関数
// 線分構造体
struct Segment {
   D3DXVECTOR2 s; // 始点
   D3DXVECTOR2 v; // 方向ベクトル（線分の長さも担うので正規化しないように！）
};

// 2Dベクトルの外積
float D3DXVec2Cross( D3DXVECTOR2* v1, D3DXVECTOR2* v2 ) {
   return v1->x * v2->y - v1->y * v2->x;
}

// 線分の衝突
bool ColSegments(
   Segment &seg1,          // 線分1
   Segment &seg2,          // 線分2
   float* outT1 = 0,       // 線分1の内分比（出力）
   float* outT2 = 0,       // 線分2の内分比（出力
   D3DXVECTOR2* outPos = 0 // 交点（出力）
) {

   D3DXVECTOR2 v = seg2.s - seg1.s;
   float Crs_v1_v2 = D3DXVec2Cross( &seg1.v, &seg2.v );
   if ( Crs_v1_v2 == 0.0f ) {
      // 平行状態
      return false;
   }

   float Crs_v_v1 = D3DXVec2Cross( &v, &seg1.v );
   float Crs_v_v2 = D3DXVec2Cross( &v, &seg2.v );

   float t1 = Crs_v_v2 / Crs_v1_v2;
   float t2 = Crs_v_v1 / Crs_v1_v2;

   if ( outT1 )
      *outT1 = Crs_v_v2 / Crs_v1_v2;
   if ( outT2 )
      *outT2 = Crs_v_v1 / Crs_v1_v2;

   const float eps = 0.00001f;
   if ( t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1 ) {
      // 交差していない
      return false;
   }

   if( outPos )
      *outPos = seg1.s + seg1.v * t1;

   return true;
}
  */
  
  
  return false;
}

//src:line dst:circle
static bool chkLineCircle( CDATA *src, CDATA *dst ) {
  return false;  
}

//src:circle dst:circle
static bool chkCircleCircle( CDATA *src, CDATA *dst ) {
  return false;  
}



//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
