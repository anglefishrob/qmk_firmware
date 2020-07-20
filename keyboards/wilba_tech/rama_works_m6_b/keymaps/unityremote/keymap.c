// A remote for Unity
// qmk compile -kb wilba_tech/rama_works_m6_b -km unityremote





//TODO This doesnt fully work (RGB not changeable). Use the VIA tool with the firmware from VIA






#include QMK_KEYBOARD_H
#include "quantum.h"

enum planck_layers {
  DefaultLayer,
  WinLayer,
  RGBLayer,
};

#define MAC_PLAY_FN LT(RGBLayer, LCMD(KC_P))
#define MAC_PAUSE LCMD(LSFT(KC_P))
#define MAC_STEP LCMD(LALT(KC_P))
#define MAC_SCENE LCMD(KC_1)
#define MAC_GAME LCMD(KC_2)
#define MAC_SCREENSHOT LCMD(LSFT(KC_5))

#define WIN_PLAY_FN LT(RGBLayer, LCTL(KC_P))
#define WIN_PAUSE LCTL(LSFT(KC_P))
#define WIN_STEP LCTL(LALT(KC_P))
#define WIN_SCENE LCTL(KC_1)
#define WIN_GAME LCTL(KC_2)
#define WIN_SCREENSHOT LGUI(LSFT(KC_S))

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[DefaultLayer] = LAYOUT(
	    RGB_MOD, RGB_TOG, MAC_STEP, 
	    MAC_SCENE, MAC_GAME, MAC_SCREENSHOT
	),

	[WinLayer] = LAYOUT(
	    WIN_PLAY_FN,  WIN_PAUSE, WIN_STEP, 
	    WIN_SCENE, WIN_GAME, WIN_SCREENSHOT
	),

	[RGBLayer] = LAYOUT(
	    _______,  TO(WinLayer), RGB_TOG, 
	    TO(DefaultLayer), RGB_HUI, KC_NO
	),
};

