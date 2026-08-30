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

#include "krsplit.h"
#include "split_util.h"
#include "transactions.h"
#include "features/autocorrection.h"

// void matrix_init_kb(void) {
//     setPinOutput(LED_CAPS_LOCK_PIN);
//     matrix_init_user();
// }

// bool led_update_kb(led_t led_state) {
//     writePin(LED_CAPS_LOCK_PIN, !led_state.caps_lock);
//     return true;
// }

#ifdef POINTING_DEVICE_ENABLE
void pointing_device_init_kb(void) {
    pointing_device_init_user();
}
#endif

#if defined(POINTING_DEVICE_ENABLE) || defined(KRSPLIT_ENCODER_MODE_MAP_ENABLE)
typedef union {
    uint16_t raw;
    struct {
        uint8_t pointer_default_dpi : 4; // 16 steps available.
        uint8_t pointer_sniping_dpi : 2; // 4 steps available.
        uint8_t enc_modes[NUM_ENCODERS];
        bool    is_dragscroll_enabled : 1;
        bool    is_sniping_enabled : 1;
    } __attribute__((packed));
} krsplit_config_t;

static krsplit_config_t g_krsplit_config = {0};

/**
 * \brief Set the value of `config` from EEPROM.
 *
 * Note that `is_dragscroll_enabled` and `is_sniping_enabled` are purposefully
 * ignored since we do not want to persist this state to memory.  In practice,
 * this state is always written to maximize write-performances.  Therefore, we
 * explicitly set them to `false` in this function.
 */
static void read_krsplit_config_from_eeprom(krsplit_config_t* config) {
    config->raw                   = eeconfig_read_kb() & 0xffff;
    config->is_dragscroll_enabled = false;
    config->is_sniping_enabled    = false;
}

/**
 * \brief Save the value of `config` to eeprom.
 *
 * Note that all values are written verbatim, including whether drag-scroll
 * and/or sniper mode are enabled.  `read_krsplit_config_from_eeprom(…)`
 * resets these 2 values to `false` since it does not make sense to persist
 * these across reboots of the board.
 */
static void write_krsplit_config_to_eeprom(krsplit_config_t* config) {
    eeconfig_update_kb(config->raw);
}

// void eeconfig_init_kb(void) {
// #ifdef BACKLIGHT_ENABLE
//     backlight_enable();
//     backlight_level(3);
// #endif

//     eeconfig_update_kb(0);
//     eeconfig_init_user();
// }


void eeconfig_init_kb(void) {
    #ifdef BACKLIGHT_ENABLE
        backlight_enable();
        backlight_level(3);
    #endif
    
    g_krsplit_config.raw                 = 0;
    g_krsplit_config.pointer_default_dpi = 4;

#    ifdef KRSPLIT_ENCODER_MODE_MAP_ENABLE
    for (size_t i = 0; i < NUM_ENCODERS; i++) {
        if (krsplit_encoder_mode_map[i][0].initalized) {
            krsplit_enc_mode_t* first_enc_mode = &krsplit_encoder_mode_map[i][0];
            g_krsplit_config.enc_modes[i]      = first_enc_mode->mode;
        }
    }
#    endif // KRSPLIT_ENCODER_MODE_MAP_ENABLE

    write_krsplit_config_to_eeprom(&g_krsplit_config);
    eeconfig_init_user();
}

void matrix_init_kb(void) {
    read_krsplit_config_from_eeprom(&g_krsplit_config);
    matrix_init_user();
    
}

void krsplit_config_sync_handler(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(g_krsplit_config)) {
        memcpy(&g_krsplit_config, initiator2target_buffer, sizeof(g_krsplit_config));
    }
}

void keyboard_post_init_kb(void) {
    transaction_register_rpc(RPC_ID_KB_CONFIG_SYNC, krsplit_config_sync_handler);
    keyboard_post_init_user();
    rgblight_sethsv_at(HSV_RED, 2); // led 2
}

void housekeeping_task_kb(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to slave.
        static krsplit_config_t last_krsplit_config = {0};
        static uint32_t        last_sync          = 0;
        bool                   needs_sync         = false;

        // Check if the state values are different.
        if (memcmp(&g_krsplit_config, &last_krsplit_config, sizeof(g_krsplit_config))) {
            needs_sync = true;
            memcpy(&last_krsplit_config, &g_krsplit_config, sizeof(g_krsplit_config));
        }
        // Send to slave every 500ms regardless of state change.
        if (timer_elapsed32(last_sync) > 500) {
            needs_sync = true;
        }

        // Perform the sync if requested.
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_KB_CONFIG_SYNC, sizeof(g_krsplit_config), &g_krsplit_config)) {
                last_sync = timer_read32();
            }
        }
    }
    // No need to invoke the user-specific callback, as it's been called
    // already.
}
#endif // defined(POINTING_DEVICE_ENABLE) || defined(KRSPLIT_ENCODER_MODE_MAP_ENABLE)

/**
 * \brief Handle the encoder mode action when triggered by encoder_update_kb
 *
 * Weakly defined fuction intended to be overridden in a users keymap
 */
__attribute__((weak)) void krsplit_encoder_mode_trigger(uint8_t mode, bool clockwise) {}

typedef struct {
    uint8_t            index;
    krsplit_enc_mode_t* enc_mode;
} krsplit_found_enc_mode_t;

static krsplit_found_enc_mode_t krsplit_get_found_encoder_mode(krsplit_config_t* config, uint8_t index) {
    krsplit_found_enc_mode_t found_enc_mode;

    for (size_t i = 0; i < KRSPLIT_ENCODER_MODE_COUNT; i++) {
        krsplit_enc_mode_t* cur_enc_mode = &krsplit_encoder_mode_map[index][i];
        if (cur_enc_mode->mode == config->enc_modes[index]) {
            found_enc_mode.index    = i;
            found_enc_mode.enc_mode = cur_enc_mode;
            break;
        }
    }

    return found_enc_mode;
}

/**
 * \brief Step through the defined encoder modes for the encoder at the given
 * index
 *
 * Step though the modes defined in krsplit_encoder_mode_map at the users keymap.
 * Use a null terminator at the first character on the name property for the
 * enc_mode struct to determine if we've reached the end of the defined encoder
 * modes. When this happens loop back to the beginning.
 */
static void krsplit_step_encoder_mode(krsplit_config_t* config, uint8_t index) {
    krsplit_found_enc_mode_t cur_enc_mode  = krsplit_get_found_encoder_mode(config, index);
    krsplit_enc_mode_t*      next_enc_mode = &krsplit_encoder_mode_map[index][(cur_enc_mode.index + 1) % KRSPLIT_ENCODER_MODE_COUNT];

    if (!next_enc_mode->initalized) {
        next_enc_mode = &krsplit_encoder_mode_map[index][0];
    }

    if (next_enc_mode->initalized) {
        config->enc_modes[index] = next_enc_mode->mode;
        write_krsplit_config_to_eeprom(config);
    }
}

static uint8_t krsplit_get_encoder_mode(krsplit_config_t* config, uint8_t index) {
    krsplit_found_enc_mode_t found_enc_mode = krsplit_get_found_encoder_mode(config, index);
    return found_enc_mode.enc_mode->mode;
}

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }

    krsplit_encoder_mode_trigger(g_krsplit_config.enc_modes[index], clockwise);

    return true;
}

enum krsplit_enc_mode {
    VOLUME,                 //볼륨 조절 엔코더 레이어
    SCROLL,                 //스크롤 조절 엔코더 레이어
    SELECT,                 //텍스트 선택 엔코더 레이어
    BACKLIGHT,              //백라이트 밝기 조절 엔코더 레이어
};

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    if (!process_autocorrection(keycode, record)) { return false; }
#ifdef KRSPLIT_ENCODER_MODE_MAP_ENABLE
    switch (keycode) {
        case ENC_MODE_STEP_RIGHT:
            if (record->event.pressed) {
                krsplit_step_encoder_mode(&g_krsplit_config, 1);
                switch (krsplit_get_encoder_mode(&g_krsplit_config, 1)) {
                    case VOLUME:
                        rgblight_sethsv_at(HSV_RED, 2); // led 2
                        break;
                    case SCROLL:
                        rgblight_sethsv_at(HSV_GREEN, 2); // led 2
                        break;
                    case SELECT:
                        rgblight_sethsv_at(HSV_WHITE, 2); // led 2
                        break;
                    case BACKLIGHT:     
                        rgblight_sethsv_at(HSV_YELLOW, 2); // led 2
                        break;
                    default:
                        rgblight_sethsv_at(HSV_RED, 2); // led 2
                        break;
                }
            }
            break;
    }
#endif // KRSPLIT_ENCODER_MODE_MAP_ENABLE

    return true;
}