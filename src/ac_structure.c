#include "ac_structure.h"

#include "m_play.h"
#include "m_name_table.h"
#include "m_malloc.h"
#include "m_common_data.h"

static void aSTR_actor_ct(ACTOR* actor, GAME* game);
static void aSTR_actor_dt(ACTOR* actor, GAME* game);
static void aSTR_actor_move(ACTOR* actor, GAME* game);

ACTOR_PROFILE Structure_Profile = {
  mAc_PROFILE_STRUCTURE,
  ACTOR_PART_CONTROL,
  ACTOR_STATE_NO_MOVE_WHILE_CULLED | ACTOR_STATE_NO_DRAW_WHILE_CULLED,
  EMPTY_NO,
  ACTOR_OBJ_BANK_KEEP,
  sizeof(STRUCTURE_CONTROL_ACTOR),

  &aSTR_actor_ct,
  &aSTR_actor_dt,
  &aSTR_actor_move,
  NONE_ACTOR_PROC,
  NULL
};

static u8 aSTR_overlay[aSTR_ACTOR_TBL_COUNT][aSTR_OVERLAY_SIZE];
static STRUCTURE_ACTOR aSTR_actor_cl[aSTR_ACTOR_TBL_COUNT];

static void aSTR_init_clip_area();
static void aSTR_free_clip_area();

static void aSTR_actor_ct(ACTOR* actor, GAME* game) {
  STRUCTURE_CONTROL_ACTOR* structure = (STRUCTURE_CONTROL_ACTOR*)actor;
  
  aSTR_init_clip_area();
  structure->str_door_name = Common_Get(door_data).door_actor_name;
  structure->reset = Common_Get(door_data).exit_type;
}

static void aSTR_actor_dt(ACTOR* actor, GAME* game) {
  aSTR_free_clip_area();
}

static int request[2]; // TODO: fix ordering by using separate TUs
static void aSTR_check_door_data(STRUCTURE_CONTROL_ACTOR* actor, GAME* game) {
  //static int request[2] = { 4, 5 };

  GAME_PLAY* play = (GAME_PLAY*)game;

  if (ITEM_NAME_GET_TYPE(actor->str_door_name) == NAME_TYPE_STRUCT) {
    STRUCTURE_ACTOR* str_actor = (STRUCTURE_ACTOR*)Actor_info_fgName_search(&play->actor_info, actor->str_door_name, ACTOR_PART_ITEM);

    if (str_actor != NULL && str_actor->request_type == 0) {
      str_actor->request_type = request[actor->reset == TRUE];
      actor->str_door_name = EMPTY_NO;
    }
  }
  else {
    actor->str_door_name = EMPTY_NO;
  }
}

static void aSTR_actor_move(ACTOR* actor, GAME* game) {
  STRUCTURE_CONTROL_ACTOR* structure = (STRUCTURE_CONTROL_ACTOR*)actor;
  switch (mFI_GetFieldId()) {
    case mFI_FIELD_FG:
      aSTR_check_door_data(structure, game);
      break;
  }
}

extern u16* structure_pal_adrs_nowinter[];
extern u16* structure_pal_adrs_winter[];

static u16* aSTR_get_pal_segment(s16 structure_type) {
  static u16** pal_adrs_table[2] = { structure_pal_adrs_nowinter, structure_pal_adrs_winter };
  
  int winter = Common_Get(time).season == mTM_SEASON_WINTER;
  return pal_adrs_table[winter][structure_type];
}

typedef struct structure_setup_info_s {
  s16 profile;
  s16 structure_type; // aSTR_TYPE_*
  s16 pal_no; // aSTR_PAL_*
  s16 pad;
} aSTR_setup_info;

static STRUCTURE_ACTOR* aSTR_setupActor_proc(GAME* game, mActor_name_t structure_name, s16 arg, f32 x, f32 z) {
  static aSTR_setup_info setupInfo_table[] = {
    { mAc_PROFILE_MYHOUSE, aSTR_TYPE_MYHOME, aSTR_PAL_MYHOME_A, 0 },
    { mAc_PROFILE_MYHOUSE, aSTR_TYPE_MYHOME, aSTR_PAL_MYHOME_B, 0 },
    { mAc_PROFILE_MYHOUSE, aSTR_TYPE_MYHOME, aSTR_PAL_MYHOME_C, 0 },
    { mAc_PROFILE_MYHOUSE, aSTR_TYPE_MYHOME, aSTR_PAL_MYHOME_D, 0 },
    { mAc_PROFILE_SHOP, aSTR_TYPE_SHOP1, aSTR_PAL_SHOP1, 0 },
    { mAc_PROFILE_CONVENI, aSTR_TYPE_SHOP2, aSTR_PAL_SHOP2, 0 },
    { mAc_PROFILE_SUPER, aSTR_TYPE_SHOP3, aSTR_PAL_SHOP3, 0 },
    { mAc_PROFILE_DEPART, aSTR_TYPE_SHOP4, aSTR_PAL_SHOP4, 0 },
    { mAc_PROFILE_POST_OFFICE, aSTR_TYPE_POST_OFFICE, aSTR_PAL_POST_OFFICE, 0 },
    { mAc_PROFILE_STATION, aSTR_TYPE_STATION1, aSTR_PAL_STATION1_A, 0 },
    { mAc_PROFILE_TRAIN0, aSTR_TYPE_TRAIN0, aSTR_PAL_TRAIN1_A1, 0 },
    { mAc_PROFILE_TRAIN1, aSTR_TYPE_TRAIN1, aSTR_PAL_TRAIN1_A2, 0 },
    { mAc_PROFILE_POLICE_BOX, aSTR_TYPE_POLICE_BOX, aSTR_PAL_POLICE_BOX, 0 },
    { mAc_PROFILE_FALLS, aSTR_TYPE_FALLS, aSTR_PAL_FALLS, 0 },
    { mAc_PROFILE_FALLSESW, aSTR_TYPE_FALLSESW, aSTR_PAL_FALLSESW, 0 },
    { mAc_PROFILE_FALLSESW, aSTR_TYPE_FALLSESW, aSTR_PAL_FALLSESW, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
    { mAc_PROFILE_SHRINE, aSTR_TYPE_SHRINE, aSTR_PAL_SHRINE, 0 },
    { mAc_PROFILE_BRSHOP, aSTR_TYPE_BRSHOP, aSTR_PAL_BR_SHOP, 0 },
    { mAc_PROFILE_BUGGY, aSTR_TYPE_BUGGY, aSTR_PAL_BUGGY, 0 },
    { mAc_PROFILE_S_CAR, aSTR_TYPE_S_CAR, aSTR_PAL_S_CAR, 0 },
    { mAc_PROFILE_KAMAKURA, aSTR_TYPE_KAMAKURA, aSTR_PAL_KAMAKURA, 0 },
    { mAc_PROFILE_GOZA, aSTR_TYPE_GOZA, aSTR_PAL_GOZA, 0 },
    { mAc_PROFILE_GOZA, aSTR_TYPE_GOZA, aSTR_PAL_GOZA, 0 },
    { mAc_PROFILE_RADIO, aSTR_TYPE_RADIO, aSTR_PAL_RADIO, 0 },
    { mAc_PROFILE_YATAI, aSTR_TYPE_YATAI, aSTR_PAL_YATAI, 0 },
    { mAc_PROFILE_YATAI, aSTR_TYPE_YATAI, aSTR_PAL_YATAI, 0 },
    { mAc_PROFILE_TUKIMI, aSTR_TYPE_TUKIMI, aSTR_PAL_TUKIMI, 0 },
    { mAc_PROFILE_TUKIMI, aSTR_TYPE_TUKIMI2, aSTR_PAL_TUKIMI2, 0 },
    { mAc_PROFILE_MIKUJI, aSTR_TYPE_MIKUJI, aSTR_PAL_MIKUJI, 0 },
    { mAc_PROFILE_COUNT, aSTR_TYPE_COUNT, aSTR_PAL_COUNT, 0 },
    { mAc_PROFILE_COUNT02, aSTR_TYPE_COUNT02, aSTR_PAL_COUNT02, 0 },
    { mAc_PROFILE_TAMA, aSTR_TYPE_SPORTSFAIR_A, aSTR_PAL_KAGO_R, 0 },
    { mAc_PROFILE_TAMA, aSTR_TYPE_SPORTSFAIR_B, aSTR_PAL_KAGO_W, 0 },
    { mAc_PROFILE_KAGO, aSTR_TYPE_SPORTSFAIR_A, aSTR_PAL_KAGO_R, 0 },
    { mAc_PROFILE_KAGO, aSTR_TYPE_SPORTSFAIR_B, aSTR_PAL_KAGO_W, 0 },
    { mAc_PROFILE_TURI, aSTR_TYPE_TURI, aSTR_PAL_TURI, 0 },
    { mAc_PROFILE_TURI, aSTR_TYPE_TURI2, aSTR_PAL_TURI2, 0 },
    { mAc_PROFILE_KOINOBORI, aSTR_TYPE_KOINOBORI, aSTR_PAL_KOINOBORI_A, 0 },
    { mAc_PROFILE_DUMP, aSTR_TYPE_DUMP, aSTR_PAL_DUMP, 0 },
    { mAc_PROFILE_WINDMILL, aSTR_TYPE_WINDMILL, aSTR_PAL_WINDMILL_A, 0 },
    { mAc_PROFILE_WINDMILL, aSTR_TYPE_WINDMILL, aSTR_PAL_WINDMILL_B, 0 },
    { mAc_PROFILE_WINDMILL, aSTR_TYPE_WINDMILL, aSTR_PAL_WINDMILL_C, 0 },
    { mAc_PROFILE_WINDMILL, aSTR_TYPE_WINDMILL, aSTR_PAL_WINDMILL_D, 0 },
    { mAc_PROFILE_WINDMILL, aSTR_TYPE_WINDMILL, aSTR_PAL_WINDMILL_E, 0 },
    { mAc_PROFILE_LOTUS, aSTR_TYPE_LOTUS, aSTR_PAL_01_LOTUS, 0 },
    { mAc_PROFILE_MIKANBOX, aSTR_TYPE_MIKANBOX, aSTR_PAL_MIKANBOX, 0 },
    { mAc_PROFILE_DOUZOU, aSTR_TYPE_DOUZOU, aSTR_PAL_DOUZOU_DAI, 0 },
    { mAc_PROFILE_TOUDAI, aSTR_TYPE_TOUDAI, aSTR_PAL_TOUDAI, 0 },
    { mAc_PROFILE_GHOG, aSTR_TYPE_GHOG, aSTR_PAL_MUSEUM, 0 },
    { mAc_PROFILE_HTABLE, aSTR_TYPE_HTABLE, aSTR_PAL_MUSEUM, 0 },
    { mAc_PROFILE_HTABLE, aSTR_TYPE_HTABLE, aSTR_PAL_MUSEUM, 0 },
    { mAc_PROFILE_HTABLE, aSTR_TYPE_HTABLE, aSTR_PAL_MUSEUM, 0 },
    { mAc_PROFILE_TENT, aSTR_TYPE_KAMAKURA, aSTR_PAL_KAMAKURA, 0 },
    { mAc_PROFILE_MUSEUM, aSTR_TYPE_MUSEUM, aSTR_PAL_MUSEUM, 0 },
    { mAc_PROFILE_BRIDGE_A, aSTR_TYPE_BRIDGE_A, aSTR_PAL_BRIDGE_A, 0 },
    { mAc_PROFILE_BRIDGE_A, aSTR_TYPE_BRIDGE_A, aSTR_PAL_BRIDGE_A, 0 },
    { mAc_PROFILE_NEEDLEWORK_SHOP, aSTR_TYPE_NEEDLEWORK_SHOP, aSTR_PAL_TAILOR, 0 },
    { mAc_PROFILE_FLAG, aSTR_TYPE_FLAG, aSTR_PAL_FLAG, 0 },
    { mAc_PROFILE_BOAT, aSTR_TYPE_BOAT, aSTR_PAL_BOAT, 0 },
    { mAc_PROFILE_COTTAGE_MY, aSTR_TYPE_COTTAGE_MY, aSTR_PAL_MYHOME_ISLAND, 0 },
    { mAc_PROFILE_COTTAGE_NPC, aSTR_TYPE_COTTAGE_NPC, aSTR_PAL_HOUSE_I, 0 },
    { mAc_PROFILE_RESERVE, aSTR_TYPE_RESERVE, aSTR_PAL_RESERVE, 0 },
  };

  STRUCTURE_ACTOR* str_actor;
  GAME_PLAY* play = (GAME_PLAY*)game;
  s16 profile;
  xyz_t pos;

  if (structure_name < NPC_HOUSE_END) {
    mNpc_SearchAnimalinfo(Save_Get(animals), NPC_HOUSE_ID_TO_NPC_ID(structure_name), ANIMAL_NUM_MAX);
    profile = mAc_PROFILE_HOUSE;
  }
  else if (structure_name >= HOUSE0 && structure_name < SHOP0) {
    int idx = structure_name - STRUCTURE_START;
    profile = setupInfo_table[idx].profile;
  }
  else if (structure_name >= TRAIN_STATION && structure_name < TRAIN0) {
    profile = mAc_PROFILE_STATION;
  }
  else {
    int idx = structure_name - STRUCTURE_START;
    profile = setupInfo_table[idx].profile;
  }

  pos.x = x;
  pos.z = z;
  pos.y = mCoBG_GetBgY_OnlyCenter_FromWpos2(pos, 0.0f);

  str_actor = (STRUCTURE_ACTOR*)Actor_info_make_actor(&play->actor_info, game, profile, pos.x, pos.y, pos.z, 0, 0, 0, play->block_table.block_x, play->block_table.block_z, -1, structure_name, arg, -1, -1);
  if (str_actor != NULL) {
    mFI_SetFG_common(RSV_NO, pos, FALSE);
  }

  return str_actor;
}

static int aSTR_get_overlay_free_area_idx() {
  aSTR_Clip_c* clip = Common_Get(clip).structure_clip;
  aSTR_Overlay_info_c* overlay_info = clip->overlay_area;
  int res = -1;
  int i;

  for (i = 0; i < aSTR_ACTOR_TBL_COUNT; i++) {
    if (overlay_info->used == FALSE) {
      res = i;
      break;
    }
    overlay_info++;
  }

  return res;
}

static void aSTR_get_overlay_area_proc(ACTOR_DLFTBL* dlftbl) {
  int overlay_idx = aSTR_get_overlay_free_area_idx();

  if (overlay_idx != -1) {
    aSTR_Overlay_info_c* overlay_info = Common_Get(clip).structure_clip->overlay_area + overlay_idx;
    aSTR_Overlay_info_c* overlay2_info;
    dlftbl->alloc_buf = overlay_info->overlay_p;

    overlay2_info = Common_Get(clip).structure_clip->overlay_area + overlay_idx;
    overlay2_info->used = TRUE;
  }
  else {
    dlftbl->alloc_buf = NULL;
  }
}

static void aSTR_free_overlay_area_proc(ACTOR_DLFTBL* dlftbl) {
  aSTR_Clip_c* clip = Common_Get(clip).structure_clip;
  aSTR_Overlay_info_c* overlay_info = clip->overlay_area;
  int i;

  for (i = 0; i < aSTR_ACTOR_TBL_COUNT; i++) {
    if (overlay_info->overlay_p == dlftbl->alloc_buf) {
      overlay_info->used = FALSE;
      dlftbl->alloc_buf = NULL;
      break;
    }
    overlay_info++;
  }
}

static STRUCTURE_ACTOR* aSTR_get_actor_area_proc() {
  aSTR_Clip_c* clip = Common_Get(clip).structure_clip;
  STRUCTURE_ACTOR** str_tbl = clip->structure_actor_tbl;
  int* str_used_tbl = clip->structure_actor_used_tbl;

  STRUCTURE_ACTOR* res = NULL;
  int i;

  for (i = 0; i < aSTR_ACTOR_TBL_COUNT; i++) {
    if (*str_tbl != NULL && *str_used_tbl == FALSE) {
      *str_used_tbl = TRUE;
      res = *str_tbl;
      break;
    }
    str_tbl++;
    str_used_tbl++;
  }

  return res;
}

static void aSTR_free_actor_area_proc(STRUCTURE_ACTOR* str_actor) {
  aSTR_Clip_c* clip = Common_Get(clip).structure_clip;
  STRUCTURE_ACTOR** str_tbl = clip->structure_actor_tbl;
  int* str_used_tbl = clip->structure_actor_used_tbl;
  int i;

  for (i = 0; i < aSTR_ACTOR_TBL_COUNT; i++) {
    if (*str_tbl == str_actor) {
      *str_used_tbl = FALSE;
      break;
    }
    str_tbl++;
    str_used_tbl++;
  }
}

static void aSTR_init_clip_area() {
  if (Common_Get(clip).structure_clip == NULL) {
    int i;

    Common_Set(clip.structure_clip, (aSTR_Clip_c*)zelda_malloc(sizeof(aSTR_Clip_c)));

    Common_Get(clip).structure_clip->setup_actor_proc = &aSTR_setupActor_proc;
    Common_Get(clip).structure_clip->get_overlay_area_proc = &aSTR_get_overlay_area_proc;
    Common_Get(clip).structure_clip->free_overlay_area_proc = &aSTR_free_overlay_area_proc;
    Common_Get(clip).structure_clip->get_actor_area_proc = &aSTR_get_actor_area_proc;
    Common_Get(clip).structure_clip->free_actor_area_proc = &aSTR_free_actor_area_proc;
    Common_Get(clip).structure_clip->get_pal_segment_proc = &aSTR_get_pal_segment;

    {
      STRUCTURE_ACTOR** str_tbl = Common_Get(clip).structure_clip->structure_actor_tbl;
      int* str_used_tbl = Common_Get(clip).structure_clip->structure_actor_used_tbl;
      aSTR_Overlay_info_c* overlay_info = Common_Get(clip).structure_clip->overlay_area;
      for (i = 0; i < aSTR_ACTOR_TBL_COUNT; i++) {
        *str_tbl = &aSTR_actor_cl[i];
        *str_used_tbl = FALSE;

        str_tbl++;
        str_used_tbl++;
    
        overlay_info->overlay_p = aSTR_overlay[i];
        overlay_info->used = FALSE;

        overlay_info++;
      }
    }
  }
}

static void aSTR_free_clip_area() {
  aSTR_Clip_c* clip = Common_Get(clip).structure_clip;

  if (clip != NULL) {
    STRUCTURE_ACTOR** str_tbl = clip->structure_actor_tbl;
    aSTR_Overlay_info_c* overlay_info = clip->overlay_area;
    int i;

    for (i = 0; i < aSTR_ACTOR_TBL_COUNT; i++) {
      if (*str_tbl != NULL) {
        *str_tbl = NULL;
      }

      if (overlay_info->overlay_p != NULL) {
        overlay_info->overlay_p = NULL;
      }

      str_tbl++;
      overlay_info++;
    }

    zelda_free(Common_Get(clip).structure_clip);
    Common_Set(clip.structure_clip, NULL);
  }
}

static int request[2] = { 4, 5 }; // TODO: remove when separated TUs are implemented and aSTR_check_door_data is corrected
