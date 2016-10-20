#pragma once

#include "drawAnim.h"
#include "collision.h"

#define MAX_EFFOBJ 32
#define MAX_ENEOBJ 48
#define MAX_PLOBJ  12

typedef enum {
  OBJ_TYPE_UNDEFINED = -1,
  OBJ_TYPE_PLAYER, //自機関連
  OBJ_TYPE_EFFECT, //自機関連と当たり判定がない
  OBJ_TYPE_ENEMY,  //自機関連と当たり判定がある
  OBJTYPEMAX,
} OBJTYPE;


struct _OBJDATA;
typedef void (*OBJFUNC_SETUP)( struct _OBJDATA* data );
typedef void (*OBJFUNC_FINISH)( struct _OBJDATA* data );
typedef void (*OBJFUNC_UPDATE)( struct _OBJDATA* data );
typedef void (*OBJFUNC_DRAW)( Layer *tgt, GContext *ctx, struct _OBJDATA* data );
typedef void (*OBJFUNC_HIT)( struct _OBJDATA* data, struct _OBJDATA* tgt );

typedef struct _OBJDATA {
  int16_t nextempty;
  int16_t own;
  int16_t next;
  int16_t prev;
  OBJTYPE type;
  GPoint pos;
  DRAWANIM *drawanim;
  CDATA  cdata;
  int8_t life;
  int8_t state; //汎用ステート
  int16_t wait; //汎用ウエイト
  int16_t cnt;  //汎用カウンタ
  int16_t prm[4]; //汎用ワーク
  OBJFUNC_SETUP funcSetup;
  OBJFUNC_FINISH funcFinish;
  OBJFUNC_UPDATE funcUpdate;
  OBJFUNC_DRAW funcDraw;
  OBJFUNC_HIT funcHit;
  void *param;
} OBJDATA;


void objInit();
void objEnd();
void objUpdate();
void objDraw( Layer *tgt, GContext *ctx );

OBJDATA* objAdd( OBJTYPE type, OBJFUNC_SETUP fsetup );
void objDel( OBJDATA *data );

void objCollisionAll();
