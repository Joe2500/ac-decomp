#ifndef M_KANKYO_H
#define M_KANKYO_H

#include "types.h"
#include "m_lights.h"

#ifdef __cplusplus
extern "C" {
#endif

enum weather {
  mEnv_WEATHER_CLEAR,
  mEnv_WEATHER_RAIN,
  mEnv_WEATHER_SNOW,
  mEnv_WEATHER_SAKURA,
  mEnv_WEATHER_LEAVES,
  
  mEnv_WEATHER_NUM
};

enum weather_intensity {
  mEnv_WEATHER_INTENSITY_NONE,
  mEnv_WEATHER_INTENSITY_LIGHT,
  mEnv_WEATHER_INTENSITY_NORMAL,
  mEnv_WEATHER_INTENSITY_HEAVY,

  mEnv_WEATHER_INTENSITY_NUM,
};

typedef void (*NATURE_PROC)(ACTOR*);

typedef struct nature_s {
  NATURE_PROC proc;
  void* arg;
} Nature;

typedef struct kankyo_s {
  /* 0x00 */ Lights sun_light;
  /* 0x0E */ u8 pad[0x9A - 0x0E];
  /* 0x9A */ u8 ambientColor[3];
  /* 0x9E */ u8 pad2[0xC8 - 0x9E];  
  /* 0xC8 */ Nature nature;
} Kankyo;

extern void Global_kankyo_ct(GAME_PLAY*, Kankyo*);
extern int mEnv_NowWeather();
extern int mEnv_WindMove();
extern void mEnv_ManagePointLight(GAME_PLAY*, Kankyo*, Global_light*);
extern void Global_kankyo_set(GAME_PLAY*, Kankyo*, Global_light*);

#ifdef __cplusplus
}
#endif

#endif
