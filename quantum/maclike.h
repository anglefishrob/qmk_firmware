#pragma once

#include QMK_KEYBOARD_H
#include "quantum.h"

typedef enum{
  WINCMD = (SAFE_RANGE),
  WINSPC,
  WINALT,
  WINTAB,
  WINLEFT,
  WINRIGHT,
  WINBKSP,
  TOWIN,
  TOMAC
}custom_keycodes;

typedef enum{
  TD_COLQUOT = 0
}tapdancees;

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_COLQUOT] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_QUOT)
};

#define COLQUOT TD(TD_COLQUOT)