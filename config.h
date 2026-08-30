#pragma once

/* ──────────────────────────────
 * MATRIX CONFIGURATION
 * ──────────────────────────────
 */
#define MATRIX_ROWS 12 // Rows are doubled-up
#define MATRIX_COLS 10
#define DIODE_DIRECTION COL2ROW

/* Left side matrix */
#define MATRIX_ROW_PINS { GP9, GP8, GP7, GP6, GP5, GP4 }
#define MATRIX_COL_PINS { NO_PIN, NO_PIN, NO_PIN, GP16, GP17, GP18, GP19, GP20, GP21, GP22 }

/* Right side matrix */
#define MATRIX_ROW_PINS_RIGHT { GP19, GP20, GP21, GP22, GP23, GP24 }
#define MATRIX_COL_PINS_RIGHT { GP4, GP5, GP6, GP7, GP8, GP9, GP10, GP11, GP12, GP13 }

/* ──────────────────────────────
 * ENCODER SETTINGS
 * ──────────────────────────────
 */
#define ENCODER_A_PINS { NO_PIN }
#define ENCODER_B_PINS { NO_PIN }
#define ENCODER_A_PINS_RIGHT { GP16 }
#define ENCODER_B_PINS_RIGHT { GP17 }
#define ENCODER_RESOLUTION 4

/* ──────────────────────────────
 * SPLIT / HANDEDNESS SETTINGS
 * ──────────────────────────────
 */
// #define MASTER_RIGHT
#define SPLIT_HAND_PIN GP3
#define SPLIT_USB_DETECT
#define SPLIT_LED_STATE_ENABLE
#define DEBOUNCE 5

/* ──────────────────────────────
 * RGB LIGHT (WS2812)
 * ──────────────────────────────
 */
#define WS2812_DI_PIN GP29
#define RGBLIGHT_LED_COUNT 4
#define RGBLIGHT_SPLIT
#define RGBLED_SPLIT { 2, 2 }

#define RGBLIGHT_SLEEP
#define RGBLIGHT_LIMIT_VAL 255
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8

#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_RAINBOW_SWIRL

/* ──────────────────────────────
 * BACKLIGHT SETTINGS
 * ──────────────────────────────
 */
#define BACKLIGHT_PIN GP28
#define BACKLIGHT_PWM_DRIVER PWMD6
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A
#define BACKLIGHT_LEVELS 7
#define BACKLIGHT_DEFAULT_LEVEL 3
#define BACKLIGHT_BREATHING
#define BREATHING_PERIOD 6

#define BACKLIGHT_PIN_RIGHT GP28
#define BACKLIGHT_PWM_DRIVER_RIGHT PWMD6
#define BACKLIGHT_PWM_CHANNEL_RIGHT RP2040_PWM_CHANNEL_A

/* 유선 키보드라 USB suspend 시 절전 기능이 필요 없음.
 * 이게 없으면 호스트가 USB를 suspend 시킬 때마다 backlight_level_noeeprom(0)으로
 * 백라이트가 꺼졌다가, wake 될 때 backlight_init()으로 즉시(페이드 없이) 원래
 * 밝기로 복원되면서 반짝임 현상이 발생함 (호스트의 USB 절전/selective suspend
 * 타이밍에 따라 몇 분 간격으로 재발 가능). */
#define NO_SUSPEND_POWER_DOWN

/* ──────────────────────────────
 * SPLIT COMMUNICATION SETTINGS
 * ──────────────────────────────
 */
#define SOFT_SERIAL_PIN GP1
#define SELECT_SOFT_SERIAL_SPEED 0
#define SPLIT_USB_TIMEOUT 2000
#define SPLIT_USB_TIMEOUT_POLL 10

#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_POINTING_ENABLE
#define POINTING_DEVICE_RIGHT
#define SPLIT_TRANSACTION_IDS_KB RPC_ID_KB_CONFIG_SYNC

/* ──────────────────────────────
 * JOYSTICK / POINTING DEVICE
 * ──────────────────────────────
 */
#define ANALOG_JOYSTICK_X_AXIS_PIN GP26
#define ANALOG_JOYSTICK_Y_AXIS_PIN GP27
#define JOYSTICK_AXIS_COUNT 2
#define JOYSTICK_BUTTON_COUNT 1

#define POINTING_DEVICE_INVERT_X
#define POINTING_DEVICE_INVERT_Y
#define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#define AUTO_MOUSE_TIME 350
#define AUTO_MOUSE_DEFAULT_LAYER 2
#define MOUSEKEY_MAX_SPEED 20

/* ──────────────────────────────
 * BOOTMAGIC / RESET
 * ──────────────────────────────
 */
#define BOOTMAGIC_LITE_ROW_RIGHT 6
#define BOOTMAGIC_LITE_COLUMN_RIGHT 9

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP17
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U