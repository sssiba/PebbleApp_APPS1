#include <pebble.h>
#include "main.h"
#include "flow.h"

#include "flowGame.h"
#include "flowTitle.h"
#include "flowGameOver.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static FLOWID g_curflow;
static FLOWID g_nextflow;
static FLOWFUNC_SETUP g_funcsetup;
static FLOWFUNC_FINISH g_funcfinish;
static FLOWFUNC_UPDATE g_funcupdate;
static FLOWFUNC_DRAW g_funcdraw;

static void setupFlow( FLOWID flow );

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
void flowInit() {
  g_curflow = g_nextflow = FLOW_UNDEFINED;
}

void flowEnd() {
  //実行中の flow があれば終了
  if( g_curflow != FLOW_UNDEFINED ) {
    g_funcfinish();
  }
}

void flowUpdate() {
  //次の flow が指定されていれば現在のものを終了し、切り替え
  if( g_nextflow != FLOW_UNDEFINED ) {
    if( g_funcfinish ) g_funcfinish(); //現在の flow 終了
    setupFlow( g_nextflow );
    g_curflow = g_nextflow;
    g_nextflow = FLOW_UNDEFINED;
    g_funcsetup(); //次の flow 開始
  }
  if( g_curflow != FLOW_UNDEFINED ) {
    g_funcupdate();
  }  
}

void flowDraw( Layer* tgt, GContext* ctx ) {
//  APPLOG( "flowDraw %d", g_curflow );
  if( g_curflow != FLOW_UNDEFINED ) {
    g_funcdraw( tgt, ctx );
  }
}

static void setupFlow( FLOWID flow ) {
  switch( flow ) {
    case FLOW_TITLE:
      g_funcsetup = flowTitleSetup;
      g_funcfinish = flowTitleFinish;
      g_funcupdate = flowTitleUpdate;
      g_funcdraw = flowTitleDraw;
      break;
    case FLOW_GAME:
      g_funcsetup = flowGameSetup;
      g_funcfinish = flowGameFinish;
      g_funcupdate = flowGameUpdate;
      g_funcdraw = flowGameDraw;
      break;
    case FLOW_GAMEOVER:
      g_funcsetup = flowGameOverSetup;
      g_funcfinish = flowGameOverFinish;
      g_funcupdate = flowGameOverUpdate;
      g_funcdraw = flowGameOverDraw;
      break;
    default:
      APPLOG_E( "Undefined Flow %d", flow );
      break;
  }
}

//----------------------------------------------------------------
void flowChange( FLOWID newflow ) {
  g_nextflow = newflow;
}

