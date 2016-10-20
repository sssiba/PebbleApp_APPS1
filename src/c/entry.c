#include <pebble.h>
#include "main.h"
#include "screen.h"
#include "entry.h"
#include "object.h"

#include "objRock.h"
#include "objEnemy1.h"

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
static uint16_t g_wait;
static uint16_t g_nextrock;
static uint16_t g_nextene1;


//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
void entryInit() {
  g_wait = 0;
  g_nextrock = 10 + rand() % 100;
  g_nextene1 = 10 + rand() % 100;
}


void entryEnd() {
  
}

void entryUpdate() {
  if( ++g_wait >= g_nextrock ) {
    GRect* sr = scrGetRect();
    g_wait = 0;
    if( (rand() % 100) < 20 ) {
      g_nextrock = 12 + rand() % 15;
    } else {
      g_nextrock = 120 + rand() % 300;
    }
    
    GPoint p;
    p.x = TOFIX( sr->origin.x + sr->size.w + 5 );
    p.y = TOFIX( GROUND_Y );
    createRockS( p );
    
    p.x = TOFIX( 16 + rand() % 100 );
    p.y = TOFIX( -8 );
    createEne1( p );
  }  
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------


