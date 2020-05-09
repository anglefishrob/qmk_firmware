/* Copyright 2018 MechMerlin
 * Copyright 2018 Logan Huskins
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#define MAC_LAYER 0
#define WIN_LAYER 1
#define FN_LAYER 2

#define ESC_FN LT(2,KC_ESC)

static bool wincmdpressed = false;
static bool winaltpressed = false;

enum custom_keycodes {
  WINCMD = (SAFE_RANGE),
  WINALT,
  WINTAB,
  WINLEFT,
  WINRIGHT
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case WINCMD:
      if (record -> event.pressed) {
        wincmdpressed = true;
        register_code(KC_RCTL);
      } 
      else {
        wincmdpressed = false;
        unregister_code(KC_RCTL);
      }
      break;

    case WINALT:
      if (record -> event.pressed) {
        winaltpressed = true;
        register_code(KC_LALT);
      } 
      else {
        winaltpressed = false;
        unregister_code(KC_LALT);
      }
      break;

    case WINTAB:
      if(record -> event.pressed){
        if(wincmdpressed == true){
          // Send a single Alt+Tab instead of Ctrl+Tab
          unregister_code(KC_RCTL);
          SEND_STRING(SS_LALT(SS_TAP(X_TAB)));
          register_code(KC_RCTL);
        }
        else{
          // Regular Tab
          register_code(KC_TAB);
        }
      }
      else{
        unregister_code(KC_TAB);
      }
      break;

    case WINLEFT:
      if(record -> event.pressed){
        if(wincmdpressed == true && winaltpressed == false){
          // Send a single Home instead of Ctrl+Arrow
          unregister_code(KC_RCTL);
          SEND_STRING(SS_TAP(X_HOME));
          register_code(KC_RCTL);
        }
        else if(winaltpressed == true && wincmdpressed == false){
          // Send a single Ctrl+Arrow instead of Alt+Arrow
          unregister_code(KC_LALT);
          SEND_STRING(SS_RCTL(SS_TAP(X_LEFT)));
          register_code(KC_LALT);
        }
        else{
          // Regular Arrow
          register_code(KC_LEFT);
        }
      }
      else{
        unregister_code(KC_LEFT);
      }
      break;

    case WINRIGHT:
      if(record -> event.pressed){
        if(wincmdpressed == true && winaltpressed == false){
          // Send a single End instead of Ctrl+Arrow
          unregister_code(KC_RCTL);
          SEND_STRING(SS_TAP(X_END));
          register_code(KC_RCTL);
        }
        else if(winaltpressed == true && wincmdpressed == false){
          // Send a single Ctrl+Arrow instead of Alt+Arrow
          unregister_code(KC_LALT);
          SEND_STRING(SS_RCTL(SS_TAP(X_RIGHT)));
          register_code(KC_LALT);
        }
        else{
          // Regular Arrow
          register_code(KC_RIGHT);
        }
      }
      else{
        unregister_code(KC_RIGHT);
      }
      break;
  }
  return true;
};


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
  [MAC_LAYER] = LAYOUT_60_ansi(
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
    ESC_FN, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
    KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_UP,
    KC_LCTL, KC_LALT, KC_LCMD, KC_SPC, KC_RCMD, KC_LEFT, KC_DOWN, KC_RIGHT
  ),

/* Windows
   * ,-----------------------------------------------------------------------------------------.
   * | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] |    [v]    |
   * |-----------------------------------------------------------------------------------------+
   * | WinTab | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] |  [v]   |
   * |-----------------------------------------------------------------------------------------+
   * |   [v]   | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] |     [v]     |
   * |-----------------------------------------------------------------------------------------+
   * |    [v]    | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] | [v] |      [v]        |
   * |-----------------------------------------------------------------------------------------+
   * | [v]  | WinAlt | WinCmd |              [v]               |  [v]  |WinLeft| [v]  |WinRight|
   * `-----------------------------------------------------------------------------------------'
   */
  [WIN_LAYER] = LAYOUT_60_ansi(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    WINTAB, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, WINALT, WINCMD, _______, _______, WINLEFT, _______, WINRIGHT
  ),

  /* Function
   * ,-----------------------------------------------------------------------------------------.
   * |  `  | F1  | F2  | F3  | F4  | F5  | F6  | F7  | F8  | F9  | F10 | F11 | F12 |    Del    |
   * |-----------------------------------------------------------------------------------------+
   * |        |     | Windows|     |Reset|     |     |     |     |     |   |Screen B+|Screen B-|
   * |-----------------------------------------------------------------------------------------+
   * |   [v]   |Apple|Sat+ |Effect+|Mode+|Bright+|Hue+ | Prev |Play |Next |     |     |        |
   * |-----------------------------------------------------------------------------------------+
   * |   Shift   |   |Sat-|Effect-|Mode-|Bright-|Hue- |Mute |Vol- | Vol+ |     |               |
   * |-----------------------------------------------------------------------------------------+
   * | Ctrl  |  Alt   | Cmd  |                                 |       |       |       |       |
   * `-----------------------------------------------------------------------------------------'
   */
  [FN_LAYER] = LAYOUT_60_ansi(
    KC_GRV, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_DEL,
    XXXXXXX, XXXXXXX, TO(WIN_LAYER), XXXXXXX, RESET, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BRID, KC_BRIU, XXXXXXX,
    _______, TO(MAC_LAYER), RGB_SAI, RGB_SPI, RGB_MOD, RGB_VAI, RGB_HUI, KC_MRWD, KC_MPLY, KC_MFFD, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_LSFT, XXXXXXX, RGB_SAD, RGB_SPD, RGB_RMOD, RGB_VAD, RGB_HUD, KC_MUTE, KC_VOLD, KC_VOLU, KC_PGDN, XXXXXXX,
    KC_LCTL, KC_LALT, KC_LCMD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
  ),
};
