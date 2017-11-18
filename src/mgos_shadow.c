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

const struct mgos_shadow *mgos_shadow_get(void) {
  return s_shmadow;
}

void mgos_shadow_set(const struct mgos_shadow *ptr) {
  s_shmadow = ptr;
}

bool mgos_shadow_init(void) {
  return true;
}
