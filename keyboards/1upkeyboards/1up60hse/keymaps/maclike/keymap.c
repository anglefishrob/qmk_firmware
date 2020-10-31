// Maclike by Robert Quinn
//
// Maclike is a keymap that emulates the muscle memory and shortcuts from macOS in Windows.
//
// The modifiers on a Mac are | Ctrl | Alt | Cmd |
//
// On the Windows layer, Cmd becomes (right) Ctrl as well. 
//
// Some special cases are:
//     Task Switcher: Cmd + Tab emulates Alt + Tab
//     Home/End: Cmd + Left/Right does Home/End, which Windows does not have a shortcut for already
//     Move cursor by word: Alt + Left/Right emulates Ctrl + Left/Right
//
// Maclike also intentionally disables tapping Alt to focus the menu bar in Windows.

// Compile: make 1upkeyboards/1up60hse:maclike

#include QMK_KEYBOARD_H
#include "maclike.h"

enum planck_layers {
  DefaultLayer,
  FnLayer,
};

#define ESC_FN LT(FnLayer, KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  /* Apple macOS
   * ,-----------------------------------------------------------------------------------------.
   * | Esc |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  | Backspace |
   * |-----------------------------------------------------------------------------------------+
   * |  Tab   |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |    \   |
   * |-----------------------------------------------------------------------------------------+
   * |  Esc/Fn |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter    |
   * |-----------------------------------------------------------------------------------------+
   * |   Shift   |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |       Up        |
   * |-----------------------------------------------------------------------------------------+
   * | Ctrl  |  Alt  |  Cmd  |              Space              |  Cmd  |  Left  | Down | Right |
   * `-----------------------------------------------------------------------------------------'
   */
  [DefaultLayer] = LAYOUT_60_ansi(
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
    ESC_FN, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
    KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_UP,
    KC_LCTL, KC_LALT, KC_LCMD, KC_SPC, KC_RCMD, KC_LEFT, KC_DOWN, KC_RIGHT
  ),

  /* Function
   * ,-----------------------------------------------------------------------------------------.
   * |  `  | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 | F11 | F12 |    Del    |
   * |-----------------------------------------------------------------------------------------+
   * |        |     | Windows|     |Reset|     |     |   |   |   |   |Screen B+|Screen B-|WFEnt|
   * |-----------------------------------------------------------------------------------------+
   * |   [v]   |Apple|Sat+ |Effect+|Mode+|Bright+| Hue+ | Prev |Play |Next |    | scroll D|    |
   * |-----------------------------------------------------------------------------------------+
   * |   Shift   |    | Sat-|Effect-|Mode-|Bright-| Hue- |Mute |Vol- | Vol+ |scroll U| mouse Up|
   * |-----------------------------------------------------------------------------------------+
   * | Ctrl  |  Alt   | Cmd  |           mouse 1               |       |mouse L|mouse D|mouse R|
   * `-----------------------------------------------------------------------------------------'
   */
  [FnLayer] = LAYOUT_60_ansi(
    KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
    XXXXXXX, XXXXXXX, TOWIN, XXXXXXX, RESET, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MACBRID, MACBRIU, XXXXXXX,
    _______, TOMAC, RGB_SAI, RGB_SPI, RGB_MOD, RGB_VAI, RGB_HUI, KC_MRWD, KC_MPLY, KC_MFFD, XXXXXXX, KC_WH_U, _______,
    KC_LSFT, XXXXXXX, RGB_SAD, RGB_SPD, RGB_RMOD, RGB_VAD, RGB_HUD, KC_MUTE, KC_VOLD, KC_VOLU, KC_WH_D, KC_MS_U,
    KC_LCTL, KC_LALT, KC_LCMD, KC_BTN1, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R
  ),
};
