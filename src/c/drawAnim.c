#include <pebble.h>
#include "main.h"
#include "drawAnim.h"


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
typedef struct {
  int16_t max;
  int16_t nextempty;
//  int16_t top;
//  int16_t bottom;
  DRAWANIM *data;
} DALIST;

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
#define MAX_LAYER 1
static DALIST s_dalist[ MAX_LAYER ]; //layer0, layer1 等増やす？

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static DRAWANIM* getEmptyDrawAnim();
static void drawParts( Layer *lyr, GContext *ctx, GPoint pos, DDATA* dd );

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void drawAnimInit() {
  int tbl[ MAX_LAYER ];
  tbl[0] = MAX_DRAWANIM;
  
  for( int i=0; i<MAX_LAYER; i++ ) {
    s_dalist[i].max = tbl[i];
    s_dalist[i].nextempty = 0;
    s_dalist[i].data = malloc( sizeof( DRAWANIM ) * tbl[i] );
    memset( s_dalist[i].data, 0, sizeof( DRAWANIM ) * tbl[i] );
    for( int j=0; j<tbl[i]; j++ ) {
      DRAWANIM* da = &s_dalist[i].data[j];
      da->nextempty = j+1;
    }
  }
}

void drawAnimEnd() {
  for( int i=0; i<MAX_LAYER; i++ ) {
    free( s_dalist[i].data );
  }
}

void drawAnimUpdate( DRAWANIM *da ) {
  if( !da->adata ) return; //アニメーション情報が無い？
  
  ADATA* a = da->adata[ da->animidx ];
  if( ++da->frame >= a->frame ) {
    //更新
    switch( a->cmd ) {
      case ACMD_END: //もう終わり
        da->frame = DEF_ANIM_END;
        break;
      case ACMD_JUMP: //別の場所へ
        {
          da->animidx = a->next;
          ADATA *na = da->adata[ da->animidx ];
          da->ddset = na->ddset;
          da->frame = 0;
        }
        break;
    }    
  }
}

void drawAnimDraw( Layer *tgt, GContext *ctx, DRAWANIM* da ) {
  DDSET* dds = da->ddset;
  
  for( int i=0; i<dds->ddcount; i++ ) {
    DDATA* dd = dds->set[i];
    drawParts( tgt, ctx, da->pos, dd );
  }
}

void drawParts( Layer *tgt, GContext *ctx, GPoint pos, DDATA* dd ) {
  GPoint p;
  p.x = TOINT( pos.x + dd->ofst.x );
  p.y = TOINT( pos.y + dd->ofst.y );

  graphics_context_set_stroke_color( ctx, dd->color );
  graphics_context_set_fill_color( ctx, dd->fillcolor );
  
  switch( dd->type ) {
    case DTYPE_STROKE:
      gpath_move_to( dd->info.stroke.path, p );
      gpath_rotate_to( dd->info.stroke.path, 0 );
      gpath_draw_outline( ctx, dd->info.stroke.path );
      break;
    case DTYPE_STROKE_OPEN:
      gpath_move_to( dd->info.stroke.path, p );
      gpath_rotate_to( dd->info.stroke.path, 0 );
      gpath_draw_outline_open( ctx, dd->info.stroke.path );
      break;
    case DTYPE_STROKE_FILL:
      gpath_move_to( dd->info.stroke.path, p );
      gpath_rotate_to( dd->info.stroke.path, 0 );
      gpath_draw_filled( ctx, dd->info.stroke.path );
      break;
    case DTYPE_CIRCLE:
      graphics_draw_circle( ctx, p, dd->info.circle.r );
      break;
    case DTYPE_CIRCLE_FILL:
      graphics_fill_circle( ctx, p, dd->info.circle.r );
      break;
    case DTYPE_LINE:
      {
        GPoint pe;
        pe.x = p.x + TOINT( dd->info.line.pe.x );
        pe.y = p.y + TOINT( dd->info.line.pe.y );
        graphics_draw_line( ctx, p, pe );
      }
      break;
    default:
      APPLOG_E( "drawAnim undefined type %d", dd->type );
      break;
  }

  
  
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
static DRAWANIM* getEmptyDrawAnim() {
  DRAWANIM *ret;
  DALIST *dal = &s_dalist[0];
  
  if( dal->nextempty >= dal->max ) {
    APPLOG_E( "drawanim full" );
    return NULL;
  }
  
  ret = &dal->data[ dal->nextempty ];
  //次の空きを設定しなおし
  int16_t ne = dal->nextempty;
  dal->nextempty = ret->nextempty;
  ret->nextempty = ne;

  return ret;
}


DRAWANIM *createDrawAnimWithDDSET( DDSET *ddset ) {
  DRAWANIM *ret = getEmptyDrawAnim();
  
  ret->ddset = ddset;
  ret->adata = NULL;
  ret->animnum = 0;
  ret->frame = DEF_ANIM_END; //animation end
  ret->pos = GPoint( TOFIX(0), TOFIX(0) );
  ret->rot = 0;
  
  return ret;
}

DRAWANIM *createDrawAnimWithADATA( ADATA **adata, int16_t animnum ) {
  DRAWANIM *ret = getEmptyDrawAnim();
  
  ret->ddset = adata[0]->ddset; //最初のパターンのものをセット
  ret->adata = adata;
  ret->animnum = animnum;
  ret->animidx = 0;
  ret->frame = 0;
  ret->pos = GPoint( TOFIX(0), TOFIX(0) );
  ret->rot = 0;
  
  return ret;
}

void deleteDrawAnim( DRAWANIM *danim ) {
  DALIST *dal = &s_dalist[0];

  //次の空きを設定しなおし
  int16_t ne = danim->nextempty;
  danim->nextempty = dal->nextempty;
  dal->nextempty = ne;
}

bool drawAnimIsEnd( DRAWANIM *danim ) {
  return danim->frame == DEF_ANIM_END;
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
