/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

#include "mgos_shadow_impl.h"

#include "common/cs_dbg.h"

static const struct mgos_shadow *s_shmadow = NULL;
static SLIST_HEAD(s_shadows,
                  mgos_shadow) s_shadows = SLIST_HEAD_INITIALIZER(s_shadows);

const char *mgos_shadow_event_name(enum mgos_shadow_event ev) {
  switch (ev) {
    case MGOS_SHADOW_CONNECTED:
      return "CONNECTED";
    case MGOS_SHADOW_GET_ACCEPTED:
      return "GET_ACCEPTED";
    case MGOS_SHADOW_GET_REJECTED:
      return "GET_REJECTED";
    case MGOS_SHADOW_UPDATE_ACCEPTED:
      return "UPDATE_ACCEPTED";
    case MGOS_SHADOW_UPDATE_REJECTED:
      return "UPDATE_REJECTED";
    case MGOS_SHADOW_UPDATE_DELTA:
      return "UPDATE_DELTA";
  }
  return "";
}

static bool mgos_shadow_check_init(void) {
  static bool initialised = false;
  if (initialised) return true;
  if (s_shmadow == NULL) {
    LOG(LL_ERROR, ("Device shadow impl not set"));
    return false;
  }
  return s_shmadow->init();
}

bool mgos_shadow_add_state_handler(mgos_shadow_state_handler func, void *arg) {
  if (!mgos_shadow_check_init()) return false;
  return s_shmadow->add_state_handler(func, arg);
}

bool mgos_shadow_add_error_handler(mgos_shadow_error_handler func, void *arg) {
  if (!mgos_shadow_check_init()) return false;
  return s_shmadow->add_error_handler(func, arg);
}

bool mgos_shadow_get(void) {
  if (!mgos_shadow_check_init()) return false;
  return s_shmadow->get();
}

bool mgos_shadow_updatef(uint64_t version, const char *state_jsonf, ...) {
  bool res;
  va_list ap;
  if (!mgos_shadow_check_init()) return false;
  va_start(ap, state_jsonf);
  res = s_shmadow->updatevf(version, state_jsonf, ap);
  va_end(ap);
  return res;
}

bool mgos_shadow_update(double version, const char *state_json) {
  if (!mgos_shadow_check_init()) return false;
  return mgos_shadow_updatef(version, "%s", state_json);
}

void mgos_shadow_register(struct mgos_shadow *p) {
  SLIST_INSERT_HEAD(&s_shadows, p, link);
}

bool mgos_shadow_init(void) {
  /* Select shadow implementation - whichever successfully initialises. */
  struct mgos_shadow *s;
  SLIST_FOREACH(s, &s_shadows, link) {
    if (s->init()) {
      LOG(LL_INFO, ("Using shadow implementation: %s", s->name));
      s_shmadow = s;
      break;
    }
  }
  return true;
}
