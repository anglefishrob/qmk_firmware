/* Copyright 2015-2017 Jack Humbert
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

// Compile: qmk compile -kb planck/rev6 -km maclike

#include QMK_KEYBOARD_H
#include "muse.h"
#include "quantum.h"


enum planck_layers {
  DefaultLayer,
  NumpadLayer,
  FnLayer,
  Fn1Layer,
};

#define RAISE MO(NumpadLayer)
#define RAISE1 MO(Fn1Layer)
#define ESC_FN LT(FnLayer, KC_ESC)

#define MACBRID KC_F14
#define MACBRIU KC_F15

enum {
  TD_COLQUOT = 0
};
qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_COLQUOT] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_QUOT)
};
#define COLQUOT TD(TD_COLQUOT)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Qwerty
 * ,-----------------------------------------------------------------------------------.
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |  ;'  |Enter |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |  Up  |Shift |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Ctrl | Alt  | CMD  |      |RAISE |    Space    |RAISE | Cmd  | Left | Down |Right |
 * `-----------------------------------------------------------------------------------'
 */
[DefaultLayer] = LAYOUT_planck_grid(
    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
    ESC_FN,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    COLQUOT, KC_ENT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_UP,   KC_RSFT,
    KC_LCTL, KC_LALT, KC_LCMD, XXXXXXX, RAISE,   KC_SPC,  KC_SPC,  RAISE,   KC_RCMD, KC_LEFT, KC_DOWN, KC_RGHT
),

/* Numpad and Symbols
 * ,-----------------------------------------------------------------------------------.
 * |  `   |   1  |   2  |   3  |   \  |   !  |   |  |   (  |   )  |   -  |   +  | [v]  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |   4  |   5  |   6  |      |   ?  |   &  |   {  |   }  |   /  |   *  |  =   |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |   7  |   8  |   9  |   0  |      |      |   [  |   ]  |   .  | [v]  | [v]  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  | [v]  | [v]  | [v]  | Fn1  |     [v]     | Fn1  | [v]  | [v]  | [v]  | [v]  |
 * `-----------------------------------------------------------------------------------'
 */
[NumpadLayer] = LAYOUT_planck_grid(
    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_BSLS, KC_EXLM, KC_PIPE, KC_LPRN, KC_RPRN, KC_MINS, KC_PPLS, _______,
    _______, KC_4,    KC_5,    KC_6,    XXXXXXX, KC_QUES, KC_AMPR, KC_LCBR, KC_RCBR, KC_PSLS, KC_PAST, KC_PEQL,
    _______, KC_7,    KC_8,    KC_9,    KC_0,    XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, _______, _______, _______,
    _______, _______, _______, _______, RAISE1,  _______, _______, RAISE1,  _______, _______, _______, _______
),

/* Fn
 * ,-----------------------------------------------------------------------------------.
 * |  `   |  F1  |  F2  |  F3  | Reset|      |      |      |      |      |      | Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |  F4  |  F5  |  F6  |      |      |      | Prev | Play | Next |      |WheelD|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |  F7  |  F8  |  F9  |      | ScBr-| ScBr+| Mute | Vol- | Vol+ |MouseU|WheelU|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  | [v]  | CMD  | [v]  | [v]  |   Mouse 1   | [v]  | [v]  |MouseL|MouseD|MouseR|
 * `-----------------------------------------------------------------------------------'
 */
[FnLayer] = LAYOUT_planck_grid(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_DEL,
    _______, KC_F4,   KC_F5,   KC_F6,   KC_F10,  KC_F11,  KC_F12,  KC_MRWD, KC_MPLY, KC_MFFD, XXXXXXX, KC_WH_D,
    _______, KC_F7,   KC_F8,   KC_F9,   XXXXXXX, MACBRID, MACBRIU, KC_MUTE, KC_VOLD, KC_VOLU, KC_MS_U, KC_WH_U,
    _______, _______, KC_LCMD, _______, _______, KC_BTN1, KC_BTN1, _______, _______, KC_MS_L, KC_MS_D, KC_MS_R
),

[Fn1Layer] = LAYOUT_planck_grid(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   RESET, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, CK_TOGG, XXXXXXX, XXXXXXX,  MU_MOD,  MU_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

};

#ifdef AUDIO_ENABLE
  float plover_song[][2]     = SONG(PLOVER_SOUND);
  float plover_gb_song[][2]  = SONG(PLOVER_GOODBYE_SOUND);
#endif

/*layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case COLEMAK:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_COLEMAK);
      }
      return false;
      break;
    case DVORAK:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_DVORAK);
      }
      return false;
      break;
    case BACKLIT:
      if (record->event.pressed) {
        register_code(KC_RSFT);
        #ifdef BACKLIGHT_ENABLE
          backlight_step();
        #endif
        #ifdef KEYBOARD_planck_rev5
          writePinLow(E6);
        #endif
      } else {
        unregister_code(KC_RSFT);
        #ifdef KEYBOARD_planck_rev5
          writePinHigh(E6);
        #endif
      }
      return false;
      break;
    case PLOVER:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          stop_all_notes();
          PLAY_SONG(plover_song);
        #endif
        layer_off(_RAISE);
        layer_off(_LOWER);
        layer_off(_ADJUST);
        layer_on(_PLOVER);
        if (!eeconfig_is_enabled()) {
            eeconfig_init();
        }
        keymap_config.raw = eeconfig_read_keymap();
        keymap_config.nkro = 1;
        eeconfig_update_keymap(keymap_config.raw);
      }
      return false;
      break;
    case EXT_PLV:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(plover_gb_song);
        #endif
        layer_off(_PLOVER);
      }
      return false;
      break;
  }
  return true;
}

bool muse_mode = false;
uint8_t last_muse_note = 0;
uint16_t muse_counter = 0;
uint8_t muse_offset = 70;
uint16_t muse_tempo = 50;

void encoder_update(bool clockwise) {
  if (muse_mode) {
    if (IS_LAYER_ON(_RAISE)) {
      if (clockwise) {
        muse_offset++;
      } else {
        muse_offset--;
      }
    } else {
      if (clockwise) {
        muse_tempo+=1;
      } else {
        muse_tempo-=1;
      }
    }
  } else {
    if (clockwise) {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_DOWN);
      #else
        tap_code(KC_PGDN);
      #endif
    } else {
      #ifdef MOUSEKEY_ENABLE
        tap_code(KC_MS_WH_UP);
      #else
        tap_code(KC_PGUP);
      #endif
    }
  }
}

void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0: {
#ifdef AUDIO_ENABLE
            static bool play_sound = false;
#endif
            if (active) {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_song); }
#endif
                layer_on(_ADJUST);
            } else {
#ifdef AUDIO_ENABLE
                if (play_sound) { PLAY_SONG(plover_gb_song); }
#endif
                layer_off(_ADJUST);
            }
#ifdef AUDIO_ENABLE
            play_sound = true;
#endif
            break;
        }
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
  switch (keycode) {
    case RAISE:
    case LOWER:
      return false;
    default:
      return true;
  }
}*/
