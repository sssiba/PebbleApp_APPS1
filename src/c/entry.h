#pragma once


enum {
  ETYPE_CMD_JUMP = -2,
  ETYPE_CMD_WAIT_ALLCLEAR = -1, //登場している敵が全ていなくなるまで待つ
  ETYPE_ENEMY1 = 0,
  ETYPE_ROCKS,
};


//移動パターン, HP 等もここで設定する？
typedef struct _TBLDATA {
  int16_t wait;
  int8_t  type;
  int8_t  prm;
} TBLDATA;


void entryInit();
void entryEnd();
void entryUpdate();
//void entryDraw( Layer *tgt, GContext *ctx );
void entryDecCount();
void entryDecGroundCount();
void entryIncCount();
void entryIncGroundCount();


