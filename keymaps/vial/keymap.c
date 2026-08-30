/* Copyright 2023 MinHyeong-Lee
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

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _WBASE,     // 윈도우 키맵
    _MAC,       // 맥 키맵
    _FN,        // 백라이트 컨트롤 레이어
    _MOUSE      // 조이스틱 마우스 레이어 (자동)
};

#include "features/autocorrection.h"

//마우스 포인터 구현부
#define SCROLL_TIMEOUT 25
#define DELTA_X_THRESHOLD 60
#define DELTA_Y_THRESHOLD 15

bool scroll_enabled = false;
bool lock_state     = false;

// State
static int8_t delta_x = 0;
static int8_t delta_y = 0;

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (scroll_enabled) {
        delta_x += mouse_report.x;
        delta_y += mouse_report.y;

        if (delta_x > DELTA_X_THRESHOLD) {
            mouse_report.h = 1;
            delta_x        = 0;
        } else if (delta_x < -DELTA_X_THRESHOLD) {
            mouse_report.h = -1;
            delta_x        = 0;
        }

        if (delta_y > DELTA_Y_THRESHOLD) {
            mouse_report.v = -1;
            delta_y        = 0;
        } else if (delta_y < -DELTA_Y_THRESHOLD) {
            mouse_report.v = 1;
            delta_y        = 0;
        }
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

// 엔코더: 볼륨 조절만
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 1) {  // 오른쪽 엔코더
        if (clockwise) {
            tap_code(KC_VOLU);  // 볼륨 증가
        } else {
            tap_code(KC_VOLD);  // 볼륨 감소
        }
        return false;
    }
    return true;
}

// RGB LED 구현부
#ifdef RGBLIGHT_ENABLE
extern rgblight_config_t rgblight_config;
static bool caps_lock_state = false;

void matrix_init_user(void) {
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT); // 고정 모드 설정
}

// LED 1번: Caps Lock 표시
void update_caps_led(void) {
    if (caps_lock_state) {
        rgblight_sethsv_at(HSV_WHITE, 1);  // Caps Lock ON → 흰색
    } else {
        rgblight_sethsv_at(HSV_OFF, 1);    // Caps Lock OFF → 꺼짐
    }
}

// LED 0번: 레이어 표시
void update_layer_led(void) {
    switch (get_highest_layer(layer_state)) {
        case _WBASE:
            rgblight_sethsv_at(HSV_RED, 0);    // 윈도우 → 빨간색
            break;
        case _MAC:
            rgblight_sethsv_at(HSV_GREEN, 0);  // 맥 → 초록색
            break;
        case _FN:
            rgblight_sethsv_at(HSV_CYAN, 0);   // FN → 청록색
            break;
        case _MOUSE:
            rgblight_sethsv_at(HSV_BLUE, 0);   // 마우스 → 파란색
            break;
        default:
            rgblight_sethsv_at(HSV_OFF, 0);
            break;
    }
}

// Caps Lock 상태 변경 시
bool led_update_user(led_t led_state) {
    caps_lock_state = led_state.caps_lock;
    update_caps_led();
    return true;
}

// 레이어 변경 시
layer_state_t layer_state_set_user(layer_state_t state) {
    update_layer_led();
    return state;
}

void keyboard_post_init_user(void) {
    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT); // 고정 모드
    update_caps_led();
    update_layer_led();
    rgblight_sethsv_at(HSV_OFF, 2);  // LED 2번 끄기
    rgblight_sethsv_at(HSV_OFF, 3);  // LED 3번 끄기
}

void housekeeping_task_user(void) {
    if (!is_keyboard_master()) {
        static layer_state_t last_state = 255;
        if (last_state != layer_state) {
            last_state = layer_state;
            update_layer_led();
        }
    }
}
#endif

void pointing_device_init_user(void) {
    set_auto_mouse_layer(_MOUSE); // 조이스틱 움직이면 자동으로 _MOUSE 레이어 활성화
    set_auto_mouse_enable(true);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Windows Base Layer */
    [_WBASE] = LAYOUT(
        KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,                                          KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_PSCR,    KC_NO,
        KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,                   KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_BSPC,    KC_INS,     KC_HOME,
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,                               KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_DEL,     KC_END,
        KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,                               KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,     KC_PGUP,    KC_PGDN,
        KC_LSFT,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,                   KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,                KC_RSFT,    KC_UP,
        TO(_MAC),   KC_LCTL,    KC_LGUI,    KC_LALT,                KC_SPC,                                         KC_SPC,     KC_RALT,    KC_RGUI,    MO(_FN),    KC_RCTL,    MS_BTN1, KC_LEFT,    KC_DOWN,    KC_RGHT
        ),
    
    /* Mac Base Layer */
    [_MAC] = LAYOUT(
        KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,                                          KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     KC_PSCR,    KC_NO,
        KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       KC_6,                   KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS,    KC_EQL,                 KC_BSPC,    KC_INS,     KC_HOME,
        KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,                               KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_LBRC,    KC_RBRC,    KC_BSLS,    KC_DEL,     KC_END,
        KC_CAPS,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,                               KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    KC_QUOT,                KC_ENT,     KC_PGUP,    KC_PGDN,
        KC_LSFT,                KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,                   KC_B,       KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,                KC_RSFT,    KC_UP,
        TO(_WBASE), KC_LCTL,    KC_LALT,    KC_LGUI,                KC_SPC,                                         KC_SPC,     KC_RGUI,    KC_RALT,    MO(_FN),    KC_RCTL,    MS_BTN1, KC_LEFT,    KC_DOWN,    KC_RGHT
        ),
    
    /* Mouse Layer (조이스틱 사용 시 자동 활성화) */
    [_MOUSE] = LAYOUT(
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    MS_BTN1, MS_BTN2, KC_TRNS,    KC_TRNS,    KC_TRNS,                            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,                                        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS
        ),
    
    /* FN Layer - Backlight Controls (MO(_FN) 키로만 활성화) */
    [_FN] = LAYOUT(
        QK_BOOT,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                                        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    BL_TOGG,    BL_UP,
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    BL_STEP,    BL_DOWN,
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                            KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    UG_TOGG,    UG_NEXT,
        KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    UG_VALU,
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,                                        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    UG_HUED,    UG_VALD,    UG_HUEU
        ),
};