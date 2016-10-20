#pragma once

#include <pebble.h>

void flowGameSetup();
void flowGameFinish();
void flowGameUpdate();
void flowGameDraw( Layer *tgt, GContext *ctx );

void addScore( uint32_t v );
void addLeft( int8_t v );
uint32_t getScore();
uint8_t getLeft();
