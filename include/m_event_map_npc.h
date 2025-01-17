#ifndef M_EVENT_MAP_NPC_H
#define M_EVENT_MAP_NPC_H

#include "types.h"
#include "m_actor.h"
#include "m_npc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int mEvMN_GetJointEventRandomNpc(mActor_name_t* selected_npc_id);
extern int mEvNM_CheckJointEvent();
extern void mEvMN_GetEventNpcName(mActor_name_t* npc_name, int event, int idx, int param_4);

#ifdef __cplusplus
}
#endif

#endif
