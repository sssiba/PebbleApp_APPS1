#include <pebble.h>
#include "main.h"
#include "object.h"
#include "collision.h"


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
typedef struct {
  int16_t max;
  int16_t nextempty;
  int16_t top;
  int16_t bottom;
  OBJDATA *data;
} OBJLIST;

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static OBJLIST g_list[ OBJTYPEMAX ];

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

void objInit() {
  int tbl[OBJTYPEMAX];
  tbl[0] = MAX_PLOBJ;
  tbl[1] = MAX_EFFOBJ;
  tbl[2] = MAX_ENEOBJ;
  
  for( int i=0; i<OBJTYPEMAX; i++ ) {
    g_list[i].max = tbl[i];
    g_list[i].nextempty = 0;
    g_list[i].top = -1;
    g_list[i].bottom = -1;
    g_list[i].data = malloc( sizeof( OBJDATA ) * tbl[i] );
    memset( g_list[i].data, 0, sizeof(OBJDATA) * tbl[i] );
    for( int j=0; j<tbl[i]; j++ ) {
      OBJDATA* od = &g_list[i].data[j];
      od->nextempty = j+1;
      od->type = (OBJTYPE)i;
      od->own = j;
    }
  }
}

void objEnd() {
  for( int i=0; i<OBJTYPEMAX; i++ ) {
    //実行中のものがあれば終了させる
    while( g_list[i].top >= 0 ) {
      objDel( &g_list[i].data[ g_list[i].top ] );
    }
    
    free( g_list[i].data );
  }
}

void objUpdate() {
  for( int i=0; i<OBJTYPEMAX; i++ ) {
    int c = g_list[i].top;
    while( c >= 0 ) {
      OBJDATA *od = &g_list[i].data[c];
        
      c = od->next;
        
      //object 更新
      od->funcUpdate( od );        
        
      //animation があれば更新
      if( od->drawanim ) {
        if( od->drawanim->adata ) {
          drawAnimUpdate( od->drawanim );
        }
      } 
    }
  }
}

void objDraw( Layer *tgt, GContext *ctx ) {
  for( int i=0; i<OBJTYPEMAX; i++ ) {
    int c = g_list[i].top;
    while( c >= 0 ) {
      OBJDATA *od = &g_list[i].data[c];
      c = od->next;
        
      //描画物設定ある？
      if( od->drawanim ) {
        od->drawanim->pos = od->pos;
        drawAnimDraw( tgt, ctx, od->drawanim );
      }
      //カスタム描画設定ある？
      if( od->funcDraw ) {
        od->funcDraw( tgt, ctx, od );
      }
    }
  }
}

void objCollisionAll() {
  //src にプレイヤー関連
  OBJDATA *src, *dst;
  
  OBJLIST *srclist = &g_list[ OBJ_TYPE_PLAYER ];
  int sc = srclist->top;
  while( sc >= 0 ) {
    src = &srclist->data[sc];
    sc = src->next;
    
    //当たり判定が無効なら何もしない
    if( !src->cdata.obj ) continue;
      
    //対象 object と判定
    OBJLIST *dstlist = &g_list[ OBJ_TYPE_ENEMY ];
    int dc = dstlist->top;
    while( dc >= 0 ) {
      dst = &dstlist->data[dc];
      dc = dst->next;
      
      //当たり判定が無効なら何もしない
      if( !dst->cdata.obj ) continue;
      
      //判定
      if( colCheck( &src->cdata, &dst->cdata ) ) {
        //当たった事を object に通知
        if( src->funcHit ) src->funcHit( src, dst );
        if( dst->funcHit ) dst->funcHit( dst, src );
        
        //当たったら他には当てない
        break;
      }
    }
  }
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
OBJDATA* objAdd( OBJTYPE type, OBJFUNC_SETUP fsetup ) {
  OBJLIST *list = &g_list[type];
  
  if( list->nextempty >= list->max ) {
    APPLOG_E( "obj type %d full", type );
    return NULL; //もう空きが無い
  }
  
  OBJDATA *od = &list->data[ list->nextempty ];
  
  //次の空きを設定しなおし
  int16_t ne = list->nextempty;
  list->nextempty = od->nextempty;;
  od->nextempty = ne;
  od->funcSetup = fsetup;
  od->funcDraw = NULL;
  od->funcHit = NULL;
  od->drawanim = NULL;
  
  od->cdata.obj = NULL; //当たり判定無効
  
  //実行リンク先頭に組み込み
  if( list->top < 0 ) {
    list->top = list->bottom = ne;
    od->next = od->prev = -1;
  } else {
    int prv = list->top;
    OBJDATA *pod = &list->data[ prv ];
    
    list->top = ne;
    od->next = prv;
    od->prev = -1;
    pod->prev = ne;
  }
  
  //初期化実行
  od->funcSetup( od );
  
  return od;
}

void objDel( OBJDATA *od ) {
  //後始末
  od->funcFinish( od );
  
  if( od->drawanim ) {
    deleteDrawAnim( od->drawanim );
    //update 内で delete した場合にアニメーションの更新が動かない様に消しておく
    od->drawanim = NULL;
  }
  
  OBJLIST *list = &g_list[ od->type ];

  //実行リンクから外す
  int16_t oidx = od->own;
  if( list->top == oidx ) {
    list->top = od->next;
  }
  if( list->bottom == oidx ) {
    list->bottom = od->prev;    
  }
  
  if( od->next >= 0 ) {
    list->data[ od->next ].prev = od->prev;
  }
  if( od->prev >= 0 ) {
    list->data[ od->prev ].next = od->next;
  }
  
  od->next = od->prev = -1;
  
  //次の空きを設定しなおし
  int16_t ne = od->nextempty;
  od->nextempty = list->nextempty;
  list->nextempty = ne;
}


