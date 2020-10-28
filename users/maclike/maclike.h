// Shared maclike functionality

#ifndef USERSPACE
#define USERSPACE

#pragma once

#include QMK_KEYBOARD_H
#include "quantum.h"

typedef enum{
  FNENT = (SAFE_RANGE),
  TOWIN,
  TOMAC
}custom_keycodes;

typedef enum{
  TD_COLQUOT = 0
}tapdancees;

__attribute__ ((weak)) qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_COLQUOT] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_QUOT)
};

#define COLQUOT TD(TD_COLQUOT)

#endif