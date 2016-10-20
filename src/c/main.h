#pragma once


#define APPLOG( ... ) app_log( APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__ )
#define APPLOG_D( ... ) app_log( APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__ )
#define APPLOG_W( ... ) app_log( APP_LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__ )
#define APPLOG_E( ... ) app_log( APP_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__ )

#define TOFIX( v ) ((v) * 64)
#define TOINT( v ) ((v) / 64)

/*
  int32_t ang = TRIG_MAX_ANGLE * degree / 360; <<<<< これは DEG_TO_TRIGANGLE() で置き換え可能
  (int16_t)( (sin_lookup( ang ) * スケール ) / TRIG_MAX_RATIO ) <<<<<<< TRIG_MAX_RATIO == 0xffff 

*/
#define TOFIXSIN( v, s ) ((int16_t)( (int32_t)(sin_lookup( DEG_TO_TRIGANGLE(v) ) * s ) >> 10)) //TRIG_MAX_RATIO 0xffff なので、16bit から　tofix の 64(6bit) 分除外
#define TOFIXCOS( v, s ) ((int16_t)( (int32_t)(cos_lookup( DEG_TO_TRIGANGLE(v) ) * s ) >> 10))


#define GROUND_Y 120

#define GROUND_SPD TOFIX( -1.0f )

uint32_t getFrame();
GFont getFont();
GFont getFontTelop();

GPoint toFixPos( GPoint v );
GPoint toIntPos( GPoint v );
