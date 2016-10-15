#include <pebble.h>
#include "object.h"

#include "main.h"

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
  tbl[0] = MAX_EFFOBJ;
  tbl[1] = MAX_ENEOBJ;
  
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
    if( g_list[i].top >= 0 ) {
      int c = g_list[i].top;
      do {
        OBJDATA *od = &g_list[i].data[c];
        c = od->next;
        od->funcUpdate( od );        
      } while( c >= 0);
    }
  }
}

void objDraw( Layer *tgt, GContext *ctx ) {
  for( int i=0; i<OBJTYPEMAX; i++ ) {
    if( g_list[i].top >= 0 ) {
      int c = g_list[i].top;
      do {
        OBJDATA *od = &g_list[i].data[c];
        c = od->next;
        od->funcDraw( tgt, ctx, od );        
      } while( c >= 0);
    }
  }
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
OBJDATA* objAdd( OBJTYPE type, OBJFUNC_SETUP fsetup, OBJFUNC_FINISH ffinish, OBJFUNC_UPDATE fupdate, OBJFUNC_DRAW fdraw ) {
  OBJLIST *list = &g_list[type];
  
  if( list->nextempty >= list->max ) return NULL; //もう空きが無い
  
  OBJDATA *od = &list->data[ list->nextempty ];
  
  //次の空きを設定しなおし
  int16_t ne = list->nextempty;
  list->nextempty = od->nextempty;;
  od->nextempty = ne;
  od->funcSetup = fsetup;
  od->funcFinish = ffinish;
  od->funcUpdate = fupdate;
  od->funcDraw = fdraw;
  
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
  
  OBJLIST *list = &g_list[ od->type ];

  //実行リンクから外す
  int16_t oidx = od->own;
  if( list->top == oidx ) {
    list->top = od->next;
  }
  if( list->bottom == oidx ) {
    list->bottom = od->prev;    
  }
  od->next = od->prev = -1;
  
  //次の空きを設定しなおし
  int16_t ne = od->nextempty;
  od->nextempty = list->nextempty;
  list->nextempty = ne;
}


