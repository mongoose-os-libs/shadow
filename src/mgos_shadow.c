/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

#include "mgos_shadow.h"

static const struct mgos_shadow *s_shmadow = NULL;

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

void mgos_shadow_set(const struct mgos_shadow *ptr) {
  s_shmadow = ptr;
}

bool mgos_shadow_add_state_handler(mgos_shadow_state_handler func, void *arg) {
  if (s_shmadow == NULL) return false;
  return s_shmadow->add_state_handler(func, arg);
}

bool mgos_shadow_add_error_handler(mgos_shadow_error_handler func, void *arg) {
  if (s_shmadow == NULL) return false;
  return s_shmadow->add_error_handler(func, arg);
}

bool mgos_shadow_get(void) {
  if (s_shmadow == NULL) return false;
  return s_shmadow->get();
}

bool mgos_shadow_updatef(uint64_t version, const char *state_jsonf, ...) {
  bool res;
  va_list ap;
  if (s_shmadow == NULL) return false;
  va_start(ap, state_jsonf);
  res = s_shmadow->updatevf(version, state_jsonf, ap);
  va_end(ap);
  return res;
}

bool mgos_aws_shadow_update(double version, const char *state_json) {
  if (s_shmadow == NULL) return false;
  return mgos_shadow_updatef(version, "%s", state_json);
}

bool mgos_shadow_init(void) {
  return true;
}
