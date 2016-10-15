#pragma once

typedef enum _FLOWID {
  FLOW_UNDEFINED,
  FLOW_TITLE,
  FLOW_GAME,
  FLOW_GAMEOVER,
} FLOWID;

typedef void (*FLOWFUNC_SETUP)( void );
typedef void (*FLOWFUNC_FINISH)( void );
typedef void (*FLOWFUNC_UPDATE)( void );
typedef void (*FLOWFUNC_DRAW)( Layer *tgt, GContext *ctx );


void flowInit();
void flowEnd();
void flowUpdate();
void flowDraw( Layer *tgt, GContext *ctx );

void flowChange( FLOWID newflow );
