#ifndef FAMICOM_EMU_H
#define FAMICOM_EMU_H

#include "types.h"
#include "game.h"

#ifdef __cplusplus
extern "C" {
#endif

/* sizeof(struct game_famicom_emu_s) == 0xE0 */
typedef struct game_famicom_emu_s {
  /* 0x00 */ GAME game;
} GAME_FAMICOM_EMU;

extern void* my_malloc_func[];

extern void famicom_emu_main(GAME* game);
extern void famicom_emu_init(GAME* game);
extern void famicom_emu_cleanup(GAME* game);
extern int famicom_gba_getImage(u32 rom_id, int* ptr);
extern void famicom_gba_removeImage(void* );

#ifdef __cplusplus
}
#endif

#endif
