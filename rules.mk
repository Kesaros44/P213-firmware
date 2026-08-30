# MCU name
MCU = RP2040

# SRC += oled/oled_handler.c process_record.c
# Autocorrection setting
SRC += features/autocorrection.c
AUTOCORRECTION_ENABLE = yes

# Bootloader selection
BOOTLOADER = rp2040

# RP2040-specific options
PICO_INTRINSICS_ENABLED = no # ATM Unsupported by ChibiOS.
SERIAL_DRIVER = vendor

# KEY options
# TAP_DANCE_ENABLE = yes
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
# COMBO_ENABLE = yes
ENCODER_ENABLE = yes

# LED, RGB options
BACKLIGHT_ENABLE = yes
BACKLIGHT_DRIVER = pwm
SPLIT_BACKLIGHT_ENABLE = yes  # 스플릿 양쪽 백라이트 동기화 추가
RGBLIGHT_ENABLE = yes
WS2812_DRIVER = vendor

# Firmware options
BOOTMAGIC_ENABLE = yes

# Split keyboard options
SPLIT_KEYBOARD = yes

# Joystick options
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = analog_joystick