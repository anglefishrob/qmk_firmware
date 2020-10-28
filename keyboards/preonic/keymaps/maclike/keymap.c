#include QMK_KEYBOARD_H
#include "muse.h"
#include "quantum.h"
#include "maclike.h"


enum planck_layers {
  DefaultLayer,
  NumpadLayer,
  FnLayer,
  Fn1Layer,
};

#define RAISE MO(NumpadLayer)
#define RAISE1 MO(Fn1Layer)
#define ESC_FN LT(FnLayer, KC_ESC)
#define LSTHAN S(KC_COMM)
#define GRTHAN S(KC_DOT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |  ;t' |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |  Up  |Shift |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Alt  | CMD  |      |RAISE |    Space    |RAISE | Cmd  | Left | Down |Right |
 * `-----------------------------------------------------------------------------------'
 */
[DefaultLayer] = LAYOUT_preonic_grid(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    ESC_FN,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    COLQUOT, KC_ENT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,   KC_RSFT,
    KC_LCTL, KC_LALT, KC_LCMD, XXXXXXX, RAISE,   KC_SPC,  KC_SPC,  RAISE,   KC_RCMD, KC_LEFT, KC_DOWN, KC_RGHT
),

/* Numpad and Symbols
 * ,-----------------------------------------------------------------------------------.
 * |  `   |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 | [v]  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   !  |   |  |   (  |   )  |   -  |   +  | [v]  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |   4  |   5  |   6  |      |   ?  |   &  |   {  |   }  |  /s\ |   *  |  =   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |   7  |   8  |   9  |   0  |   <  |   >  |   [  |   ]  |   .  | [v]  | [v]  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  | [v]  | [v]  | [v]  | Fn1  |     [v]     | Fn1  | [v]  | [v]  | [v]  | [v]  |
 * `-----------------------------------------------------------------------------------'
 */
[NumpadLayer] = LAYOUT_preonic_grid(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_EXLM, KC_PIPE, KC_LPRN, KC_RPRN, KC_MINS, KC_PPLS, _______,
    _______, KC_4,    KC_5,    KC_6,    XXXXXXX, KC_QUES, KC_AMPR, KC_LCBR, KC_RCBR, KC_PSLS, KC_PAST, KC_PEQL,
    _______, KC_7,    KC_8,    KC_9,    KC_0,    LSTHAN,  GRTHAN,  KC_LBRC, KC_RBRC, _______, _______, _______,
    _______, _______, _______, _______, RAISE1,  _______, _______, RAISE1,  _______, _______, _______, _______
),

/* Fn
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |      |      |      |      |      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |  `   |  F1  |  F2  |  F3  | F12  |      |      |      |      |      |WheelD| Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |  F4  |  F5  |  F6  | F11  |      |      | Prev | Play | Next |WheelU| Enter|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |  F7  |  F8  |  F9  | F10  | ScBr-| ScBr+| Mute | Vol- | Vol+ |MouseU|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  | [v]  | [v]  | [v]  |  M3  |   Mouse 1   |  M2  | [v]  |MouseL|MouseD|MouseR|
 * `-----------------------------------------------------------------------------------'
 */
[FnLayer] = LAYOUT_preonic_grid(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_WH_D, KC_DEL,
    _______, KC_F4,   KC_F5,   KC_F6,   KC_F11,  XXXXXXX, XXXXXXX, KC_MRWD, KC_MPLY, KC_MFFD, KC_WH_U, FNENT,
    _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_BRMD, KC_BRMU, KC_MUTE, KC_VOLD, KC_VOLU, KC_MS_U, _______,
    _______, _______, _______, _______, KC_BTN3, KC_BTN1, KC_BTN1, KC_BTN2, _______, KC_MS_L, KC_MS_D, KC_MS_R
),

// Keyboard functions
[Fn1Layer] = LAYOUT_preonic_grid(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX,   TOWIN, XXXXXXX,   RESET, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX,   TOMAC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, CK_TOGG, XXXXXXX, XXXXXXX,  MU_MOD,  MU_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),
};