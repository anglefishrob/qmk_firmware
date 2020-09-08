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

// Compile: qmk compile -kb 1upkeyboards/1up60hse -km maclike

#include QMK_KEYBOARD_H

enum planck_layers {
  DefaultLayer,
  WinLayer,
  FnLayer,
};

#define ESC_FN LT(2,KC_ESC)

#define MACBRID KC_F14
#define MACBRIU KC_F15

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
  [WinLayer] = LAYOUT_60_ansi(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, WINBKSP,
    WINTAB, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, WINALT, WINCMD, WINSPC, _______, WINLEFT, _______, WINRIGHT
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
    _______, TOMAC, RGB_SAI, RGB_SPI, RGB_MOD, RGB_VAI, RGB_HUI, KC_MRWD, KC_MPLY, KC_MFFD, XXXXXXX, KC_WH_D, WINFNENT,
    KC_LSFT, XXXXXXX, RGB_SAD, RGB_SPD, RGB_RMOD, RGB_VAD, RGB_HUD, KC_MUTE, KC_VOLD, KC_VOLU, KC_WH_U, KC_MS_U,
    KC_LCTL, KC_LALT, KC_LCMD, KC_BTN1, XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_R
  ),
};
