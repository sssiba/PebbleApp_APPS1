#pragma once
#include <pebble.h>

#define MAX_DRAWANIM 128

/*
  x!x! .pdc の draw command 使った方が良い感じかも
*/

typedef enum {
  DTYPE_STROKE,
  DTYPE_STROKE_OPEN,
  DTYPE_STROKE_FILL,
  DTYPE_CIRCLE,
  DTYPE_CIRCLE_FILL,
  DTYPE_LINE,
} DRAWTYPE;

typedef struct {
  GPath* path; //x!x!<< これ使い回し出来ないのかも
} INFOSTROKE;

typedef struct {
  GPoint pe;  //視点は DDATA ofst で、pe はそこからの相対位置
} INFOLINE;

typedef struct {
  int16_t r;
} INFOCIRCLE;

//１描画パーツの情報
typedef struct _DDATA {
  DRAWTYPE type;
  GPoint ofst;
  int16_t rot;
  GColor color;
  GColor fillcolor;
  union {
    INFOSTROKE stroke;
    INFOLINE   line;
    INFOCIRCLE circle;
  } info;
} DDATA;

//複数描画パーツのセット
typedef struct _DDSET {
  int ddcount;
  DDATA **set;
} DDSET;

//アニメーション情報
typedef struct _ADATA {
  int16_t frame;
  DDSET   *ddset;
  int8_t  cmd; // 00:end  01:jump(loop用)
  int8_t  next; // (jump 先等)
} ADATA;

enum {
  ACMD_END,
  ACMD_JUMP,
};

#define AC_END() ACMD_END, 0
#define AC_JUMP( _idx ) ACMD_JUMP, _idx

#define DEF_ANIM_END -1


//描画物(１つにつき１つ)
//x!x! object とセットにする？完全に分離？
//x!x! とりあえずセットでやってみる
typedef struct _DRAWANIM {
  int16_t nextempty;
  
  GPoint pos;
  int16_t rot;
  int16_t frame;
  DDSET *ddset; //描画する対象(アニメーションがある場合は、アニメーション側がここを書き換える)
  ADATA **adata;
  int8_t animnum;
  int8_t animidx;
} DRAWANIM;

void drawAnimInit();
void drawAnimEnd();
void drawAnimUpdate( DRAWANIM* da );
void drawAnimDraw( Layer *tgt, GContext *ctx, DRAWANIM *da );

DRAWANIM *createDrawAnimWithDDSET( DDSET *ddset );
DRAWANIM *createDrawAnimWithADATA( ADATA **adata, int16_t animnum );
void deleteDrawAnim( DRAWANIM *danim );

bool drawAnimIsEnd( DRAWANIM *danim );



