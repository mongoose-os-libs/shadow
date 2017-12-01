/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

/*
 * Device shadow implementation API
 */

#ifndef MGOS_SHADOW_IMPL_H
#define MGOS_SHADOW_IMPL_H

#include "mgos_shadow.h"

#include "common/queue.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Shadow API implementation descriptor */
struct mgos_shadow {
  const char *name;
  bool (*init)(void);
  bool (*add_state_handler)(mgos_shadow_state_handler, void *arg);
  bool (*add_error_handler)(mgos_shadow_error_handler, void *arg);
  bool (*get)(void);
  bool (*updatevf)(uint64_t version, const char *state_jsonf, va_list ap);

  /* Linkage of all shadow implementations into one list */
  SLIST_ENTRY(mgos_shadow) link;
};

/* Register device shadow implementation. */
void mgos_shadow_register(struct mgos_shadow *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_SHADOW_IMPL_H */
