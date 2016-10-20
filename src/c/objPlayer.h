#pragma once

#include "object.h"

void objPlayerSetup( OBJDATA *od );
void objPlayerFinish( OBJDATA *od );
void objPlayerUpdate( OBJDATA *od );
void objPlayerDraw( Layer *tgt, GContext *ctx, OBJDATA *od );


GPoint plGetPos();



//--------
void pbShootV();
void pbShootH();


void objPBVSetup( OBJDATA* od );
void objPBHSetup( OBJDATA* od );
//finish, update は縦横共通
void objPBFinish( OBJDATA* od );
void objPBUpdate( OBJDATA* od );
void objPBHit( OBJDATA *od, OBJDATA *tgt );
  