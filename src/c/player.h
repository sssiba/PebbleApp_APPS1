#pragma once

void plInit();
void plEnd();
void plUpdate();
void plDraw( Layer *tgt, GContext *ctx );

GPoint plGetPos();
void plSetPos( int x, int y );

//--------
void pbInit();
void pbEnd();
void pbUpdate();
void pbDraw( Layer *tgt, GContext * ctx );
void pbShootV();
void pbShootH();
