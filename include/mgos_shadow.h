/*
 * Copyright (c) 2014-2017 Cesanta Software Limited
 * All rights reserved
 */

/*
 * Cross-cloud device cloud state API.
 */

#ifndef MGOS_SHADOW_H
#define MGOS_SHADOW_H

#include <stdbool.h>
#include <stdint.h>

#include "common/mg_str.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum mgos_shadow_event {
  MGOS_SHADOW_CONNECTED = 0,
  MGOS_SHADOW_GET_ACCEPTED = 1,
  MGOS_SHADOW_GET_REJECTED = 2,
  MGOS_SHADOW_UPDATE_ACCEPTED = 3,
  MGOS_SHADOW_UPDATE_REJECTED = 4,
  MGOS_SHADOW_UPDATE_DELTA = 5,
};

/*
 * Main Device Shadow state callback handler.
 *
 * Will get invoked when connection is established or when new versions
 * of the state arrive via one of the topics.
 *
 * CONNECTED event comes with no state.
 *
 * For DELTA events, state is passed as "desired", reported is not set.
 */
typedef void (*mgos_shadow_state_handler)(void *arg, enum mgos_shadow_event ev,
                                          const struct mg_str *state);
typedef void (*mgos_shadow_error_handler)(void *arg, enum mgos_shadow_event ev,
                                          int code, const char *message);

/* Return ASCII name of the shadow event: "CONNECTED", "GET_REJECTED", ... */
const char *mgos_shadow_event_name(enum mgos_shadow_event ev);

struct mgos_shadow {
  /* Setup shadow state handler. */
  void (*add_state_handler)(mgos_shadow_state_handler, void *arg);

  /* Setup shadow error handler. */
  void (*add_error_handler)(mgos_shadow_error_handler, void *arg);

  /*
   * Request shadow state. Response will arrive via GET_ACCEPTED topic.
   * Note that MGOS automatically does this on every (re)connect if
   * device.shadow.get_on_connect is true (default).
   */
  bool (*get)(void);
  /*
   * Send an update. Format string should define the value of the "state" key,
   * i.e. it should be an object with "reported" and/or "desired" keys, e.g.:
   * `mgos_shadow_updatef("{reported:{foo: %d, bar: %d}}", foo, bar)`.
   * Response will arrive via UPDATE_ACCEPTED or REJECTED topic.
   * If you want the update to be aplied only if a particular version is
   * current,
   * specify the version. Otherwise set it to 0 to apply to any version.
   */
  bool (*updatef)(uint64_t version, const char *state_jsonf, ...);

  /* "Simple" version of mgos_shadow_updatef, primarily for FFI.  */
  bool (*update)(double version, const char *state_json);
};

/* Get device shadow. */
const struct mgos_shadow *mgos_shadow_get(void);

/* Set device shadow. */
void mgos_shadow_set(const struct mgos_shadow *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MGOS_SHADOW_H */
