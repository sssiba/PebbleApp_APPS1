#pragma once

typedef enum {
  COLTYPE_POINT,
  COLTYPE_SQUARE,
  COLTYPE_LINE,
  COLTYPE_CIRCLE,
} COLTYPE;

typedef struct {
  GRect rect;
} CINFOSQUARE;

typedef struct {
  GPoint ofst;
  GPoint e;
} CINFOLINE;

typedef struct {
  GPoint ofst;
  int16_t r;
} CINFOCIRCLE;


struct _OBJDATA;
typedef struct _CDATA {
  COLTYPE type;
  struct _OBJDATA *obj;
  union {
    CINFOSQUARE square;
    CINFOLINE line;
    CINFOCIRCLE circle;
  } info;
} CDATA;

bool colCheck( CDATA *src, CDATA *dst );
