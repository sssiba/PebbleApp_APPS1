#include <pebble.h>
#include "main.h"
#include "flow.h"
#include "screen.h"
#include "bg.h"
#include "drawAnim.h"

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
static void updateCB( Layer *own, GContext* ctx );

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Window *g_winParent;
Layer *g_lyrScreen;
GRect g_rectLyr;

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void scrInit( Window *win )
{
  g_winParent = win; //一応とっておく
  
  //背景を黒にする
  window_set_background_color( win, GColorBlack );
  
  //親 layer の情報取得
  Layer* winlayer = window_get_root_layer( win );
  GRect rect = layer_get_bounds( winlayer );
  
  //layer 作成
  g_rectLyr = layer_get_frame( winlayer );
  g_lyrScreen = layer_create( g_rectLyr ); //親と同じサイズ
  layer_add_child( winlayer, g_lyrScreen );
  
  //  layer_set_update_proc( Layer* layer, LayerUpdateProc update_proc ); <<<< 描画が必要な際に呼ばれるやつ
  layer_set_update_proc( g_lyrScreen, updateCB );

}

void scrEnd()
{
  layer_destroy( g_lyrScreen );
}

//---------------------------------------------------------------------
static void updateCB( Layer *tgt, GContext *ctx )
{
  flowDraw( tgt, ctx );
}

void scrDirty()
{
    layer_mark_dirty( g_lyrScreen );
}

GRect* scrGetRect() {
  return &g_rectLyr;
}

