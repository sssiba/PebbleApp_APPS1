#pragma once

#include "object.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------

void objRockSSetup( OBJDATA *od );
void objRockSFinish( OBJDATA *od );
void objRockSUpdate( OBJDATA *od );
//void objRockSDraw( Layer *tgt, GContext *ctx, OBJDATA *od );
void objRockSHit( OBJDATA *od, OBJDATA *tgt );

void createRockS();

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
