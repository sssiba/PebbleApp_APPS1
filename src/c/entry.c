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
enum {
  TBLSET_NORMAL,
  TBLSET_GROUND,
  TBLSETMAX,
};

typedef struct {
  uint16_t tblidx;
  TBLDATA* curtbl;
  uint16_t curobjcnt;
  uint16_t wait;
} TBLSET;

static TBLSET s_tblset[ TBLSETMAX ];

static TBLDATA s_tbl0[] = {
  { 30, ETYPE_ENEMY1, 0 },
  {  5, ETYPE_ENEMY1, 0 },
  {  0, ETYPE_CMD_WAIT_ALLCLEAR, 0 },
  {  5, ETYPE_ENEMY1, 0 },
  {  0, ETYPE_ENEMY1, 0 },
  {  0, ETYPE_CMD_WAIT_ALLCLEAR, 0 },
  {  0, ETYPE_CMD_JUMP, 0 },
};

static TBLDATA s_tblgrnd0[] = {
  { 60, ETYPE_ROCKS, 0 },
  { 30, ETYPE_ROCKS, 0 },
  { 45, ETYPE_ROCKS, 0 },
  { 55, ETYPE_ROCKS, 0 },
  { 90, ETYPE_ROCKS, 0 },
  {  0, ETYPE_CMD_JUMP, 0 },
};

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------


static bool cmdProcess( TBLSET *ts );
static void entryObject( TBLSET *ts );
static void entryObjEnemy1( TBLDATA *td );
static void entryObjRockS( TBLDATA *td );

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------
void entryInit() {
  for( int i=0; i<TBLSETMAX; i++ ) {
    s_tblset[i].curobjcnt = 0;
    s_tblset[i].tblidx = 0;
    s_tblset[i].wait = 0;
  }
  s_tblset[TBLSET_NORMAL].curtbl = s_tbl0;
  s_tblset[TBLSET_GROUND].curtbl = s_tblgrnd0;
}


void entryEnd() {
  
}

void entryUpdate() {

  for( int i=0; i<TBLSETMAX; i++ ) {
    TBLSET *ts = &s_tblset[i];
    ++ts->wait;
    while( ts->wait >= ts->curtbl[ ts->tblidx ].wait ) {
      ts->wait = 0;
      if( ts->curtbl[ ts->tblidx ].type < 0 ) {
        if( cmdProcess( ts ) ) {
          //index 進ませず while を抜ける
          break;          
        }
      } else {
        entryObject( ts );
      }
      ts->tblidx++;
    }  
  }
}

bool cmdProcess( TBLSET *ts ) {
  TBLDATA* td = &ts->curtbl[ ts->tblidx ];
  
  switch( td->type ) {
    case ETYPE_CMD_JUMP:
      ts->tblidx = td->prm - 1; //戻った後 +1 されるので -1 しておく
      break;
    case ETYPE_CMD_WAIT_ALLCLEAR:
      if( ts->curobjcnt > 0 ) {
        return true; //index 進ませず一旦終わる
      }
      break;
    default:
      APPLOG_E( "undefined entry cmd %d", td->type );
      break;
  }
  return false;
}

void entryObject( TBLSET *ts ) {
  TBLDATA* td = &ts->curtbl[ ts->tblidx ];
  
  switch( td->type ) {
    case ETYPE_ENEMY1:
      entryObjEnemy1( td );
      break;
    case ETYPE_ROCKS:
      entryObjRockS( td );
      break;
    default:
      APPLOG_E( "undefined entry type %d", td->type );
      break;
  }
}

void entryDecCount() {
  s_tblset[TBLSET_NORMAL].curobjcnt--;
}

void entryDecGroundCount() {
  s_tblset[TBLSET_GROUND].curobjcnt--;
}

void entryIncCount() {
  s_tblset[TBLSET_NORMAL].curobjcnt++;
}

void entryIncGroundCount() {
  s_tblset[TBLSET_GROUND].curobjcnt++;
}

void entryObjEnemy1( TBLDATA *td ) {
    GRect* sr = scrGetRect();
    GPoint p;
    p.x = TOFIX( 16 + rand() % 100 );
    p.y = TOFIX( sr->origin.y -8 );
    createEne1( p );
}

void entryObjRockS( TBLDATA *td ) {
    GRect* sr = scrGetRect();
    GPoint p;
    p.x = TOFIX( sr->origin.x + sr->size.w + 5 );
    p.y = TOFIX( GROUND_Y );
    createRockS( p );
}

//----------------------------------------------------------------
//----------------------------------------------------------------
//----------------------------------------------------------------


