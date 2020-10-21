#include QMK_KEYBOARD_H
#include "muse.h"
#include "quantum.h"
#include "maclike.h"

#define MACLIKE_WIN_LAYER 1

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

bool wintaskswitcheropen = false;
bool wincmdpressed = false;
bool winaltpressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    
    // Scrolling is 'natural' by default, reverse for windows
    case KC_WH_D:
      if(record -> event.pressed){
        if(IS_LAYER_ON(MACLIKE_WIN_LAYER)){
          register_code(KC_WH_U);
        }
        else{
          unregister_code(KC_WH_D);
        }
      }
      else{
        unregister_code(KC_WH_D);
        unregister_code(KC_WH_U);
      }
      break;
    case KC_WH_U:
      if(record -> event.pressed){
        if(IS_LAYER_ON(MACLIKE_WIN_LAYER)){
          register_code(KC_WH_D);
        }
        else{
          unregister_code(KC_WH_U);
        }
      }
      else{
        unregister_code(KC_WH_D);
        unregister_code(KC_WH_U);
      }
      break;

    case KC_ENT:
      if(record -> event.pressed){
        if(IS_LAYER_ON(MACLIKE_WIN_LAYER) && IS_LAYER_ON(FnLayer)){
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


    // Handle shifted overrides
    bool shifted = get_mods() & MOD_BIT(KC_LSHIFT) || get_mods() & MOD_BIT(KC_RSHIFT);
    case KC_PSLS:
      if(record -> event.pressed){
        if(shifted)
          register_code(KC_PSLS);
        else
          register_code(KC_BSLS);
      }
      else{
        unregister_code(KC_PSLS);
        unregister_code(KC_BSLS);
      }

      break;


    case TOWIN:
      if(record -> event.pressed){
        #ifdef MAC_SOUND
        stop_all_notes();
        PLAY_SONG(win_song);
        #endif
        
        layer_on(MACLIKE_WIN_LAYER);
      }
      break;

    case TOMAC:
      if(record -> event.pressed){
        #ifdef MAC_SOUND
        stop_all_notes();
        PLAY_SONG(mac_song);
        #endif
        
        layer_off(MACLIKE_WIN_LAYER);
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
  }
  return true;
};