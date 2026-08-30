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

#pragma once
#include "quantum.h"
#define ___ KC_NO

#ifdef KRSPLIT_ENCODER_MODE_MAP_ENABLE
#    ifndef KRSPLIT_ENCODER_MODE_COUNT
#        define KRSPLIT_ENCODER_MODE_COUNT 4
#    endif

enum spleeb_keycodes {
    POINTER_DEFAULT_DPI_FORWARD = QK_KB,
    POINTER_DEFAULT_DPI_REVERSE,
    POINTER_SNIPING_DPI_FORWARD,
    POINTER_SNIPING_DPI_REVERSE,
    SNIPING_MODE,
    SNIPING_MODE_TOGGLE,
    DRAGSCROLL_MODE,
    DRAGSCROLL_MODE_TOGGLE,
    ENC_MODE_STEP_RIGHT,
};

#define DF_MOD POINTER_DEFAULT_DPI_FORWARD
#define DF_RMOD POINTER_DEFAULT_DPI_REVERSE
#define SP_MOD POINTER_SNIPING_DPI_FORWARD
#define SP_RMOD POINTER_SNIPING_DPI_REVERSE
#define SNIPING SNIPING_MODE
#define SNP_TOG SNIPING_MODE_TOGGLE
#define DRGSCRL DRAGSCROLL_MODE
#define DRG_TOG DRAGSCROLL_MODE_TOGGLE
#define ENC_STR ENC_MODE_STEP_RIGHT

typedef struct {
    uint8_t mode;
    // Discriminate between array members which are (un)initialized
    bool initalized;
} const krsplit_enc_mode_t;

const krsplit_enc_mode_t krsplit_encoder_mode_map[NUM_ENCODERS][KRSPLIT_ENCODER_MODE_COUNT];

#    define KRSPLIT_ENC_MODE(mode) \
        { mode, true }
#endif // KRSPLIT_ENCODER_MODE_MAP_ENABLE

/* This is a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys minus blanks in the matrix.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix which includes blanks in the wired out matrix.
 */

#define LAYOUT(\
    K00, K01, K02, K03, K04, K05,                K06, K07, K08, K09, K10, K11, K12,   K13, K14,\
    K15, K16, K17, K18, K19, K20, K21,      K22, K23, K24, K25, K26, K27, K28,        K29, K30,\
    K31, K32, K33, K34, K35, K36,           K37, K38, K39, K40, K41, K42, K43, K44,   K45, K46,\
    K47, K48, K49, K50, K51, K52,           K53, K54, K55, K56, K57, K58, K59,        K60, K61,\
    K62, K63, K64, K65, K66, K67,           K68, K69, K70, K71, K72, K73, K74,        K75,\
    K76, K77, K78, K79, K80,                K81, K82, K83, K84, K85, K86,        K87, K88, K89\
    ) \
    { \
      { ___, ___, ___, K00, K01, K02, K03, K04, K05, ___ }, \
      { ___, ___, ___, K15, K16, K17, K18, K19, K20, K21 }, \
      { ___, ___, ___, K31, K32, K33, K34, K35, K36, ___ }, \
      { ___, ___, ___, K47, K48, K49, K50, K51, K52, ___ }, \
      { ___, ___, ___, K62, ___, K63, K64, K65, K66, K67 }, \
      { ___, ___, ___, K76, K77, K78, K79, ___, K80, ___ }, \
      { ___, K06, K07, K08, K09, K10, K11, K12, K13, K14 }, \
      { K22, K23, K24, K25, K26, K27, ___, K28, K29, K30 }, \
      { K37, K38, K39, K40, K41, K42, K43, K44, K45, K46 }, \
      { K53, K54, K55, K56, K57, K58, ___, K59, K60, K61 }, \
      { K68, K69, K70, K71, K72, K73, ___, K74, K75, ___ }, \
      { ___, K81, K82, K83, K84, K85, K86, K87, K88, K89 } \
    }
    