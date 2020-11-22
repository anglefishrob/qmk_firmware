// Shared maclike functionality

#ifndef USERSPACE
#define USERSPACE

#pragma once

#include QMK_KEYBOARD_H
#include "quantum.h"

typedef enum{
  MACLIKE_KEYCODES_FIRST = (SAFE_RANGE),
  FNENT,      // Enter in the Fn layer, used for sending rename on windows and mac
  SLASHES,    // '/' or '\' shifted
  DOTQUES,    // '.' or '?' shifted
  COMMEX,     // ',' or '!' shifted
  TOWIN,      // Turn on windows mode
  TOMAC,      // Turn off windows mode
  SPC_RSE,
  SPC_QMK,
  ESC_FN,
  MACLIKE_KEYCODES_LAST
}custom_keycodes;

typedef enum{
  TD_COLQUOT = 0
}tapdancees;

__attribute__ ((weak)) qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_COLQUOT] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_QUOT)
};

#define COLQUOT TD(TD_COLQUOT)

#define MACBRID KC_F14
#define MACBRIU KC_F15

enum planck_layers {
  DefaultLayer,
  RaiseLayer,
  FnLayer,
  QMKLayer,
};

#endif