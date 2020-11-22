#pragma once

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(PREONIC_SOUND)
    // #define STARTUP_SONG SONG(NO_SOUND)

    #define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND), \
                                  SONG(COLEMAK_SOUND), \
                                  SONG(DVORAK_SOUND) \
                                }
#endif

#define MIDI_BASIC

#define AUDIO_CLICKY

#define DAC_SAMPLE_MAX 30000U//65535U

// With this combination of tap & hold settings:
// Holding a specified key and touching another key will always do the held key.
// It is meant to be as responsive as a shift or other modifier.
// Tapping the key alone will always be the tap key.
// Don't forget to implement get_tapping_term
#define PERMISSIVE_HOLD
#define TAPPING_TERM 200
#define RETRO_TAPPING
#define TAPPING_TERM_PER_KEY
#define HOLDING_TERM 1