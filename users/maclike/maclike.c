#include QMK_KEYBOARD_H
#include "muse.h"
#include "quantum.h"
#include "maclike.h"

#ifdef AUDIO_ENABLE
// Mac chord 
#define MAC_SOUND E__NOTE(_FS4), E__NOTE(_AS4), Q__NOTE(_CS5), W__NOTE(_FS5),
float mac_song[][2] = SONG(MAC_SOUND);

// XP Startup
#define WIN_SOUND W__NOTE(_DS5), S__NOTE(_DS4), H__NOTE(_AS4), H__NOTE(_GS4), H__NOTE(_DS4), H__NOTE(_DS5), H__NOTE(_AS4)
float win_song[][2] = SONG(WIN_SOUND);
#endif

// In most apps on Windows, tapping Alt focuses the menu bar
// Using the WinAlt and WinLeft/WinRight buttons logically sends an Alt tap
// Which causes every other "move cursor by word" to focus the menu bar instead
// Every time we register Alt, send a 'blank' keycode which is used for literally nothing
// This prevents the Alt tap behavior when using arrow keys as well as all over Windows which is nice
#define BLANK X_F17

bool winmodeactive = false;
bool wintaskswitcheropen = false;
bool wincmdpressed = false;
bool winaltpressed = false;

typedef union {
  uint32_t raw;
  struct {
    bool windows_mode_pref :1; // One bit
  };
} user_config_t;
user_config_t user_config;

void keyboard_post_init_user(void) {

  // Read the user config from EEPROM
  user_config.raw = eeconfig_read_user();

  // Set initial value
  winmodeactive = user_config.windows_mode_pref;

  // Play startup sound based on winmode
  if(winmodeactive){
      #ifdef MAC_SOUND
      stop_all_notes();
      PLAY_SONG(win_song);
      #endif
  }
  else{
      #ifdef MAC_SOUND
      stop_all_notes();
      PLAY_SONG(mac_song);
      #endif
  }
}

void set_windows_mode_pref(bool enabled){
  user_config.windows_mode_pref = enabled;
  eeconfig_update_user(user_config.raw);
}

bool process_toggle_winmode(uint16_t keycode, bool pressed){

  if(keycode == TOWIN){
    if(pressed){
      #ifdef MAC_SOUND
      stop_all_notes();
      PLAY_SONG(win_song);
      #endif
      winmodeactive = true;
      set_windows_mode_pref(winmodeactive);
    }
    return false;
  }

  if(keycode == TOMAC){
    if(pressed){
      #ifdef MAC_SOUND
      stop_all_notes();
      PLAY_SONG(mac_song);
      #endif
      winmodeactive = false;
      set_windows_mode_pref(winmodeactive);
    }
    return false;
  }

  return true;
}

void handle_shifted_override(bool pressed, uint16_t unshiftedkey, uint16_t shiftedkey, bool isusansishiftedcode){

  bool leftshifted = get_mods() & MOD_BIT(KC_LSHIFT);
  bool rightshifted = get_mods() & MOD_BIT(KC_RSHIFT);
  bool anyshifted = leftshifted || rightshifted;
  
  // US ANSI shifted codes are just shorthand for LSFT(kc), don't toggle the shift keys
  leftshifted &= !isusansishiftedcode;
  rightshifted &= !isusansishiftedcode;

  if(pressed){
    if(anyshifted){
      if(leftshifted)
        unregister_code(KC_LSHIFT);  
      if(rightshifted)
        unregister_code(KC_RSHIFT);
      
      register_code(shiftedkey);
      unregister_code(shiftedkey);
      
      if(leftshifted)
        register_code(KC_LSHIFT);  
      if(rightshifted)
        register_code(KC_RSHIFT);
    }
    else
      register_code(unshiftedkey);
  }
  else{
    unregister_code(unshiftedkey);
    unregister_code(shiftedkey);
  }
}

bool process_special_case_key(uint16_t keycode, bool pressed){

  // Scrolling for mac needs to be reversed
  if(keycode == KC_WH_D){
    if(pressed){
      if(winmodeactive)
        register_code(KC_WH_D);
      else
        register_code(KC_WH_U);
    }
    else{
      unregister_code(KC_WH_D);
      unregister_code(KC_WH_U);
    }  
    return false;
  }
  if(keycode == KC_WH_U){
    if(pressed){
      if(winmodeactive)
        register_code(KC_WH_U);
      else
        register_code(KC_WH_D);
    }
    else{
      unregister_code(KC_WH_D);
      unregister_code(KC_WH_U);
    }
    return false;
  }

  if(keycode == COMMEX){
    handle_shifted_override(pressed, KC_COMM, KC_EXLM, true);
    return false;
  }

  if(keycode == DOTQUES){
    handle_shifted_override(pressed, KC_DOT, KC_QUES, true);
    return false;
  }

  if(keycode == SLASHES){
    handle_shifted_override(pressed, KC_PSLS, KC_BSLS, false);
    return false;
  }

  if(keycode == FNENT){
    if(pressed){
      if(winmodeactive)
        // Send rename if Windows sends Fn Enter
        SEND_STRING(SS_TAP(X_F2));
      else
        register_code(KC_ENT);
    }
    else
      unregister_code(KC_ENT);
    return false;
  }

  return true;
}

bool process_winmode_key(uint16_t keycode, bool pressed){

  if(keycode == KC_LCMD || keycode == KC_RCMD){
    wincmdpressed = pressed;
    if (pressed)
      register_code(KC_RCTL);
    else {
      unregister_code(KC_RCTL);

      if(wintaskswitcheropen == true){
        // Release the task switcher
        unregister_code(KC_LALT);
        wintaskswitcheropen = false;
      }
    }
    return false;
  }

  if(keycode == KC_LALT){
    winaltpressed = pressed;
    if (pressed) {
      register_code(KC_LALT);
      SEND_STRING(SS_TAP(BLANK));
    } 
    else
      unregister_code(KC_LALT);
    return false;
  }

  if(keycode == KC_TAB){
    if(pressed){
      if(wincmdpressed == true){
        // Swap Ctrl for Alt
        unregister_code(KC_RCTL);
        register_code(KC_LALT);
        wintaskswitcheropen = true;
      }

      // Tab in all cases
      register_code(KC_TAB);
    }
    else
      unregister_code(KC_TAB);
    return false;
  }

  if(keycode == KC_LEFT){
    if(pressed){
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
      else
        register_code(KC_LEFT);
    }
    else
      unregister_code(KC_LEFT);
    return false;
  }

  if(keycode == KC_RIGHT){
    if(pressed){
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
      else
        register_code(KC_RIGHT);
    }
    else
      unregister_code(KC_RIGHT);
    return false;
  }

  if(keycode == KC_BSPC){
    if(pressed){
      if(wincmdpressed == true){
        unregister_code(KC_RCTL);
        
        // Register Delete
        register_code(KC_DEL);

        register_code(KC_RCTL);
      }
      else
        register_code(KC_BSPC);
    }
    else{
      unregister_code(KC_BSPC);
      unregister_code(KC_DEL);
    }
    return false;
  }

  if(keycode == KC_SPC){
    if(pressed){
      if(wincmdpressed == true){
        
        // Cancel RCtrl from WINCMD, to use Ctrl here use the left one that is still available
        unregister_code(KC_RCTL);

        // Hold Win key instead. Tapping this key brings up the windows start menu, similar to spotlight search on mac.
        SEND_STRING(SS_DOWN(X_LWIN));
      }
      else
        register_code(KC_SPC);
    }
    else{

      // This should continue to work while space is held, so a Win+letter shortcut can be used without holding WINCTL
      SEND_STRING(SS_UP(X_LWIN));

      unregister_code(KC_SPC);
    }
    return false;
  }

  return true;
}

bool spaceraisetapready = false;
bool spaceqmktapready = false;
bool escfntapready = false;

bool process_instant_layer_tap(uint16_t keycode, bool pressed){
  
  if(keycode == SPC_RSE){
    if(pressed){
      spaceraisetapready = true;
      layer_on(RaiseLayer);
    }
    else{
      layer_off(RaiseLayer);
      if(spaceraisetapready){
        register_code(KC_SPC);
        unregister_code(KC_SPC);
      }
    }
    return false;
  }

  if(keycode == SPC_QMK){
    if(pressed){
      spaceqmktapready = true;
      layer_on(QMKLayer);
    }
    else{
      layer_off(QMKLayer);
      if(spaceqmktapready){
        register_code(KC_SPC);
        unregister_code(KC_SPC);
      }
    }
    return false;
  }

  if(keycode == ESC_FN){
    if(pressed){
      escfntapready = true;
      layer_on(FnLayer);
    }
    else{
      layer_off(FnLayer);
      if(escfntapready){
        register_code(KC_ESC);
        unregister_code(KC_ESC);
      }
    }
    return false;
  }

  return true;
}

bool process_record_user_maclike(uint16_t keycode, keyrecord_t *record) {

  bool pressed = record -> event.pressed;

  if(pressed){
     // On any button DOWN, invalidate all instant taps
    spaceraisetapready = false;
    spaceqmktapready = false;
    escfntapready = false;
  }

  if(!process_instant_layer_tap(keycode, pressed))
    return false;

  if(winmodeactive && !process_winmode_key(keycode, pressed))
    return false;

  if(!process_special_case_key(keycode, pressed))
    return false;
  
  if(!process_toggle_winmode(keycode, pressed))
    return false;

  return true;
};

// This can be overridden in a keymap, in which case be sure to include a call to process_record_user_maclike
__attribute__ ((weak)) bool process_record_user(uint16_t keycode, keyrecord_t *record){
  if(!process_record_user_maclike(keycode, record))
    return false;
  return true;
}