#include "preonic.h"
#include "action_layer.h"
#include "eeconfig.h"
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _COLEMAK 1
#define _DVORAK 2
#define _LOWER 3
#define _RAISE 4
#define _TYPING 8
#define _ADJUST 16

enum preonic_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  BACKLIT
};

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Default (Based on Qwerty)
 * ,-----------------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp       |
 * |------+------+------+------+------+------+------+------+------+------+------+------------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Rsft(Del)  |
 * |------+------+------+------+------+-------------+------+------+------+------+------------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  | Ralt(')    |
 * |------+------+------+------+------+------|------+------+------+------+------+------------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | Rsft(Enter)|
 * |------+------+------+------+------+------+------+------+------+------+------+------------|
 * | Esc  |Typing| Alt  | Lower| Ctrl |    Space    | Raise | Alt | RCtl | Meh  | Menu       |
 * `-----------------------------------------------------------------------------------------'
 */
[_QWERTY] = {
  {KC_GRV              , KC_1        ,    KC_2 ,    KC_3 ,    KC_4 ,    KC_5             ,    KC_6             ,    KC_7              ,    KC_8    , KC_9    ,    KC_0    , KC_BSPC},
  {MT(MOD_LSFT,KC_TAB) , KC_Q        ,    KC_W ,    KC_E ,    KC_R ,    KC_T             ,    KC_Y             ,    KC_U              ,    KC_I    , KC_O    ,    KC_P    , MT(MOD_RSFT,KC_DEL) },
  {MT(MOD_LCTL,KC_ESC) , KC_A        ,    KC_S ,    KC_D ,    KC_F ,    KC_G             ,    KC_H             ,    KC_J              ,    KC_K    , KC_L    ,    KC_SCLN , MT(MOD_RALT,KC_QUOT) },
  {KC_LSFT             , KC_Z        ,    KC_X ,    KC_C ,    KC_V ,    KC_B             ,    KC_N             ,    KC_M              ,    KC_COMM , KC_DOT  ,    KC_SLSH , MT(MOD_RSFT,KC_ENT) },
  {KC_LGUI             , TG(_TYPING) , KC_LALT , KC_LCTL , LOWER   , MT(MOD_LALT,KC_SPC) , MT(MOD_RCTL,KC_TAB) , LT(_RAISE,KC_ENT)    ,   KC_LALT  , KC_RCTL ,    KC_MEH  , KC_APP}
},

/* Raise
 * ,-----------------------------------------------------------------------------------.
 * |   F1 |   F2 |   F3 |  F4  |  F5  |   F6 |  F7  |  F8  |  F9  | F10  | F11  | F12
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |   `  |      |  END |      |      |      |      |      |      |      |  Up  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl |  F1  |  F2  |  F3  | RIGHT|  F5  |  F6  |   -  |   =  |   [  |   ]  |  \   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |  F7  |  F8  |  F9  |  F11 |  LEFT|  DOWN|ISO # |   <  |   >  |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      | Lower|      |             |      | Next | Vol- | Vol+ | Play |
 * `-----------------------------------------------------------------------------------'
 */
[_RAISE] = {
  {KC_F1,     KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10,  KC_F11, KC_F12 },
  {KC_GRV,  _______, _______,  KC_END, _______, _______, _______, _______,_______,    _______,   KC_UP, KC_BSPC},
  {KC_LCTL, KC_HOME, _______,  KC_DEL, KC_RGHT, _______, _______,  KC_MINS, KC_EQL,   KC_LBRC, KC_RBRC, KC_BSLS},
  {_______, _______, _______, _______, KC_PGDN, KC_LEFT, KC_DOWN,  KC_NUHS,  RSFT(KC_COMMA), RSFT(KC_DOT), _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______ }
},

/* Adjust (Lower + Raise)
 * ,-----------------------------------------------------------------------------------.
 * |      |      |      |Aud on|AudOff|AGnorm|AGswap|Qwerty|Colemk|Dvorak|      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      | Reset|      |      |LMouse|      |      |Rmouse|      |      |      |  Del |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * |      |Voice-|Voice+|Mus on|MusOff|MidiOn|MidOff|      |      |      |      |      |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * |      |      |      |      |      |             |      |      |      |      |      |
 * `-----------------------------------------------------------------------------------'
 */

[_ADJUST] = {
  {_______, _______, _______, AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  _______, _______,  _______, _______},
  {_______, RESET,   _______, _______, KC_MS_BTN1, _______, _______, KC_MS_BTN2, _______, _______, _______, KC_DEL},
  {KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12},
  {_______, MUV_DE,  MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  _______, _______, _______, _______, _______},
  {_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______}
},

/* Lower                          H1                   H2             L
 * ,------------- -----------------------------------------------------------------------------.
 * |   ~          |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |   &  |   *  |   (  |   )  | Bksp |
 * |------------- +------+------+------+------+-------------+------+------+------+------+------|
 * |   ~          |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |   &  |   *  |   (  |   )  | Del  |
 * |------------- +------+------+------+------+-------------+------+------+------+------+------|
 * |MT(LCtrl, Del)|   !  |   @  |   #  |   $  |   %  |   ^  |   _  |   +  |   {  |   }  |  |   |
 * |------------- +------+------+------+------+------|------+------+------+------+------+------|
 * |              |   F1 |   F2 |   F3 |   F4 |  F5  |   F6 | \    |   -  |   [  |   ]  | End  |
 * |------        +------+------+------+------+------+------+------+------+------+------+------|
 * |              |      |      |      |      |             |      | Next | Left | Down | Right|
 * `------        -----------------------------------------------------------------------------'
 */
[_LOWER] = {
  //                                     H1                         H2
  {KC_TILD, KC_1,     KC_2,    KC_3,     KC_4,    KC_5,    KC_6,      KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC},
  {KC_TILD, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL},
  {MT(MOD_LCTL, KC_DEL)  , KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE},
  {_______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6, KC_BSLS, KC_MINS, KC_LBRC, KC_RBRC, MT(MOD_RSFT, KC_ENT)},
  {_______, _______, _______, _______, _______, _______, _______, _______, KC_MNXT, KC_LEFT, KC_DOWN, KC_RGHT}
},


/* Typing
 * ,-----------------------------------------------------------------------------------.
 * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  | Bksp |
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Del  |
 * |------+------+------+------+------+-------------+------+------+------+------+------|
 * | Ctrl |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  "   |
 * |------+------+------+------+------+------|------+------+------+------+------+------|
 * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | Rsft(Enter)|
 * |------+------+------+------+------+------+------+------+------+------+------+------|
 * | GUI  |      | Alt  | Lower| Ctrl |    Space    | Raise | Alt | RCtl | Meh  | Menu |
 * `-----------------------------------------------------------------------------------'
 */
[_TYPING] = {
  {KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC},
  {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL },
  {MT(MOD_LCTL, KC_ESC), KC_A,KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,KC_L,KC_SCLN, KC_QUOT },
  {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  MT(MOD_RSFT, KC_ENT) },
  {KC_LGUI, _______, KC_LALT, KC_LCTL, _______,   KC_SPC,  KC_SPC,  _______,   KC_LALT, KC_RCTL, KC_MEH,  KC_APP}
}
};

#ifdef AUDIO_ENABLE
float tone_startup[][2] = {
  {NOTE_B5, 20},
  {NOTE_B6, 8},
  {NOTE_DS6, 20},
  {NOTE_B6, 8}
};

float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);

float tone_goodbye[][2] = SONG(GOODBYE_SOUND);

float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);
#endif

void persistant_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
        case QWERTY:
          if (record->event.pressed) {
            #ifdef AUDIO_ENABLE
              PLAY_NOTE_ARRAY(tone_qwerty, false, 0);
            #endif
            persistant_default_layer_set(1UL<<_QWERTY);
          }
          return false;
          break;
        case LOWER:
          if (record->event.pressed) {
            layer_on(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          } else {
            layer_off(_LOWER);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          }
          return false;
          break;
        case RAISE:
          if (record->event.pressed) {
            layer_on(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          } else {
            layer_off(_RAISE);
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
          }
          return false;
          break;
        case BACKLIT:
          if (record->event.pressed) {
            register_code(KC_RSFT);
            #ifdef BACKLIGHT_ENABLE
              backlight_step();
            #endif
          } else {
            unregister_code(KC_RSFT);
          }
          return false;
          break;
      }
    return true;
};

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
}

#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(20); // gets rid of tick
    PLAY_NOTE_ARRAY(tone_startup, false, 0);
}

void shutdown_user()
{
    PLAY_NOTE_ARRAY(tone_goodbye, false, 0);
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_NOTE_ARRAY(music_scale, false, 0);
}

#endif
