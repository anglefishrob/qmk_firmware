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
//     Ctrl + Backspace sends Delte instead 
//
// Maclike also intentionally disables tapping Alt to focus the menu bar in Windows.

// Compile: qmk compile -kb planck/rev6 -km maclike

#include QMK_KEYBOARD_H
#include "muse.h"
#include "quantum.h"


enum planck_layers {
  DefaultLayer,
  WinLayer,
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

#define MAC_SOUND E__NOTE(_FS5), E__NOTE(_AS5), S__NOTE(_REST), Q__NOTE(_CS6), Q__NOTE(_FS6),
//#define WIN_SOUND E__NOTE(_FS3), E__NOTE(_AS3), S__NOTE(_REST), Q__NOTE(_CS4), Q__NOTE(_FS4),
float mac_song[][2] = SONG(MAC_SOUND);
float win_song[][2] = SONG(QWERTY_SOUND);




//=============================//
//                             //
//                             //
//       Maclike funtions      //
//                             //
//                             //
//=============================//

// In most apps on Windows, tapping Alt focuses the menu bar
// Using the WinAlt and WinLeft/WinRight buttons logically sends an Alt tap
// Which causes every other "move cursor by word" to focus the menu bar instead
// Every time we register Alt, send a 'blank' keycode which is used for literally nothing
// This prevents the Alt tap behavior when using arrow keys as well as all over Windows which is nice
#define BLANK X_F17

static bool wintaskswitcheropen = false;
static bool wincmdpressed = false;
static bool winaltpressed = false;

enum custom_keycodes {
  WINCMD = (SAFE_RANGE),
  WINSPC,
  WINALT,
  WINTAB,
  WINLEFT,
  WINRIGHT,
  WINBKSP,
  WINFNENT,
  TOWIN,
  TOMAC
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    
    case TOWIN:
      if(record -> event.pressed){
        #ifdef MAC_SOUND
        stop_all_notes();
        PLAY_SONG(win_song);
        #endif
        
        layer_on(WinLayer);
      }
      break;

    case TOMAC:
      if(record -> event.pressed){
        #ifdef MAC_SOUND
        stop_all_notes();
        PLAY_SONG(mac_song);
        #endif
        
        layer_off(WinLayer);
      }
      break;

    case WINCMD:
      if (record -> event.pressed) {
        wincmdpressed = true;
        register_code(KC_RCTL);
      } 
      else {
        wincmdpressed = false;
        unregister_code(KC_RCTL);

        if(wintaskswitcheropen == true){
          // Release the task switcher
          unregister_code(KC_LALT);
          wintaskswitcheropen = false;
        }
      }
      break;

    case WINALT:
      if (record -> event.pressed) {
        winaltpressed = true;
        register_code(KC_LALT);
        SEND_STRING(SS_TAP(BLANK));
      } 
      else {
        winaltpressed = false;
        unregister_code(KC_LALT);
      }
      break;

    case WINTAB:
      if(record -> event.pressed){
        if(wincmdpressed == true){
          // Swap Ctrl for Alt
          unregister_code(KC_RCTL);
          register_code(KC_LALT);
          wintaskswitcheropen = true;
        }

        // Tab in all cases
        register_code(KC_TAB);
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
          SEND_STRING(SS_TAP(BLANK));
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
          SEND_STRING(SS_TAP(BLANK));
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

    case WINBKSP:
      if(record -> event.pressed){
        if(wincmdpressed == true){
          // Send a single Delete instead of Ctrl+Backspace
          unregister_code(KC_RCTL);
          SEND_STRING(SS_TAP(X_DEL));
          register_code(KC_RCTL);
        }
        else{
          // Regular Backspace
          register_code(KC_BSPC);  
        }
      }
      else{
        unregister_code(KC_BSPC);
      }
      break;

    case WINSPC:
      if(record -> event.pressed){
        if(wincmdpressed == true){
          
          // Cancel RCtrl from WINCMD, to use Ctrl here use the left one that is still available
          unregister_code(KC_RCTL);

          // Hold Win key instead. Tapping this key brings up the windows start menu, similar to spotlight search on mac.
          SEND_STRING(SS_DOWN(X_LWIN));
        }
        else{
          // Regular Space
          register_code(KC_SPC);  
        }
      }
      else{

        // This should continue to work while space is held, so a Win+letter shortcut can be used without holding WINCTL
        SEND_STRING(SS_UP(X_LWIN));

        unregister_code(KC_SPC);
      }
      break;

    case WINFNENT:
      if(record -> event.pressed){
        if(IS_LAYER_ON(WinLayer) && IS_LAYER_ON(FnLayer)){
          // Send rename if Windows sends Fn Enter
          SEND_STRING(SS_TAP(X_F2));
        }
        else{
          // Regular Enter
          register_code(KC_ENT);
        }
      }
      else{
        unregister_code(KC_ENT);
      }
      break;
  }
  return true;
};



//=============================//
//                             //
//                             //
//           Key maps          //
//                             //
//                             //
//=============================//

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

/* Windows
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
[WinLayer] = LAYOUT_planck_grid(
     WINTAB, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, WINBKSP,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  WINALT,  WINCMD, _______, _______,  WINSPC,  WINSPC, _______, _______, WINLEFT, _______, WINRIGHT
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
 * |  `   |  F1  |  F2  |  F3  | Reset|      |      |      |      |      |WheelD| Del  |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |  F4  |  F5  |  F6  |      |      |      | Prev | Play | Next |WheelU| Enter|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  |  F7  |  F8  |  F9  |      | ScBr-| ScBr+| Mute | Vol- | Vol+ |MouseU|      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | [v]  | [v]  | CMD  | [v]  | [v]  |   Mouse 1   | [v]  | [v]  |MouseL|MouseD|MouseR|
 * `-----------------------------------------------------------------------------------'
 */
[FnLayer] = LAYOUT_planck_grid(
    KC_GRV,  KC_F1,   KC_F2,   KC_F3,   RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_WH_D, KC_DEL,
    _______, KC_F4,   KC_F5,   KC_F6,   KC_F10,  KC_F11,  KC_F12,  KC_MRWD, KC_MPLY, KC_MFFD, KC_WH_U, WINFNENT,
    _______, KC_F7,   KC_F8,   KC_F9,   XXXXXXX, MACBRID, MACBRIU, KC_MUTE, KC_VOLD, KC_VOLU, KC_MS_U, XXXXXXX,
    _______, _______, KC_LCMD, _______, _______, KC_BTN1, KC_BTN1, _______, _______, KC_MS_L, KC_MS_D, KC_MS_R
),

// Keyboard functions
[Fn1Layer] = LAYOUT_planck_grid(
    XXXXXXX, XXXXXXX,   TOWIN, XXXXXXX,   RESET, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX,   TOMAC, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, CK_TOGG, XXXXXXX, XXXXXXX,  MU_MOD,  MU_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
),

};

