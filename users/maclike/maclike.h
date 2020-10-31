// Shared maclike functionality

#ifndef USERSPACE
#define USERSPACE

#pragma once

#include QMK_KEYBOARD_H
#include "quantum.h"

typedef enum{
  MACLIKE_KEYCODES_FIRST = (SAFE_RANGE),
  FNENT,      // Enter in the Fn layer, used for sending rename on windows and mac
  SLASHES,    // '/' or '\' if right shift is held
  TOWIN,      // Turn on windows mode
  TOMAC,      // Turn off windows mode
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

#define LSTHAN S(KC_COMM)
#define GRTHAN S(KC_DOT)

#endif