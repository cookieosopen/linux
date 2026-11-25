/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _CKOS_PEND_H
#define _CKOS_PEND_H

int MT_InitPendingStatus(void (*object)(void));
int MT_UnlockPendingStatus(void (*object)(void));

#endif /* _CKOS_PEND_H */