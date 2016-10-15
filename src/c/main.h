#pragma once


#define APPLOG( ... ) app_log( APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__ )
#define APPLOG_D( ... ) app_log( APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__ )
#define APPLOG_W( ... ) app_log( APP_LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__ )
#define APPLOG_E( ... ) app_log( APP_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__ )

#define TOFIX( v ) ((v) * 64)
#define TOINT( v ) ((v) / 64)

#define GROUND_Y 120

#define GROUND_SPD TOFIX( 3.0f )

uint32_t getFrame();
GFont getFont();
GFont getFontTelop();
