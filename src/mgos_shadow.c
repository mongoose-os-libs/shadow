/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */
#include "mgos_shadow.h"

#include "mgos.h"

const char *mgos_shadow_event_name(int ev) {
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

bool mgos_shadow_get(void) {
  return mgos_event_trigger(MGOS_SHADOW_GET, NULL) > 0;
}

bool mgos_shadow_updatef(uint64_t version, const char *json_fmt, ...) {
  int res;
  struct mgos_shadow_update_data data = {.json_fmt = json_fmt,
                                         .version = version};
  va_start(data.ap, json_fmt);
  res = mgos_event_trigger(MGOS_SHADOW_UPDATE, &data);
  va_end(data.ap);
  return res > 0;
}

bool mgos_shadow_update(double version, const char *state_json) {
  /*
   * Make a copy of the string. Reason: state_json may come from mJS.
   * mgos_shadow_updatef triggers a set of event handlers that can invoke mJS,
   * GC-ing and invalidating that pointer.
   */
  char *msg = strdup(state_json);
  bool res = mgos_shadow_updatef(version, "%s", msg);
  free(msg);
  return res;
}

bool mgos_shadow_init(void) {
  mgos_event_register_base(MGOS_SHADOW_BASE, "shadow");
  return true;
}
