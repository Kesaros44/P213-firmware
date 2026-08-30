# krsplit P213

An RP2040-based split custom keyboard from the krsplit series, connected over wired USB-C. It's a column/row-staggered split board designed around an ANSI 107-key layout, with a rotary encoder and an analog joystick (pointing device) on the right half, letting you move the cursor and scroll without a separate mouse.

* Keyboard Maintainer: [kesaros44](https://github.com/kesaros44)
* Hardware Supported: RP2040 (Raspberry Pi RP2040), one PCB per half (left/right)
* Hardware Availability: personal custom PCB (not publicly distributed)

## Hardware

* **MCU:** RP2040 (ARM Cortex-M0+), UF2 bootloader
* **Matrix:** 12 rows x 10 cols (6 rows used per half), `COL2ROW` diode direction
* **Split communication:** soft serial (`GP1`); `SPLIT_USB_DETECT` lets either half become master depending on which side USB is plugged into; `SPLIT_HAND_PIN` (`GP3`) determines left/right handedness
* **Rotary encoder:** one, on the right half only (`GP16`/`GP17`). The left half has the position reserved (`NO_PIN`) but no physical encoder — this keeps the left/right pin indices aligned
* **Backlight:** PWM-driven (`GP28`), 7 brightness levels, breathing effect supported (6-second cycle)
* **RGB underglow:** WS2812, 4 LEDs total (2 per half, `GP29`)
* **Pointing device:** analog joystick (X: `GP26`, Y: `GP27`) — moving the joystick automatically switches to a dedicated mouse layer (auto-reverts after 350ms of inactivity), with custom logic that switches to scroll mode past a certain tilt angle
* **Entering the bootloader:** double-tap the reset button (standard RP2040 UF2 method); the `GP17` LED indicates bootloader mode

## Software Features

* 4 layers: a Windows base layer, a Mac base layer (Cmd/Opt swapped), an Fn layer (backlight/RGB controls), and a Mouse layer (entered automatically when the joystick is used)
* Built-in autocorrection (QMK community feature, with its own dictionary data)
* Split into two builds — see `BUILD_AND_VIAL_GUIDE.txt` for the exact build commands:
  * `keymaps/default` — plain QMK build (no VIA/Vial)
  * `keymaps/vial` — built with [Vial](https://get.vial.today/) support, so keymaps/macros/combos can be edited live without recompiling firmware (must be built against the [vial-kb/vial-qmk](https://github.com/vial-kb/vial-qmk) fork — it will not build against mainline QMK)

## Important Notes (issues found firsthand)

* **The keyboard definition file must be named `keyboard.json`.** The older name, `info.json`, is no longer recognized by current QMK at all — the build simply ignores it.
* **A keyboard-level C source file is only auto-included in the build if it's named exactly after its own folder.** For this board that means `p213.c`, not `krsplit.c` — if the name doesn't match, the build can appear to succeed while silently dropping everything in that file (autocorrection wiring, RGB state sync, EEPROM init logic, etc.) with no compile error. Always double-check the filename.
* Encoder pins use the current macro names (`ENCODER_A_PINS`/`ENCODER_B_PINS`(`_RIGHT`)) — the old names (`ENCODERS_PAD_A`/`ENCODERS_PAD_B`) collide with current QMK's auto-generated code and cause a compile error.
* The keymap uses current keycode names for mouse buttons and underglow: `MS_BTN1`/`MS_BTN2` (formerly `KC_MS_BTN1`/`KC_MS_BTN2`), `UG_TOGG`/`UG_NEXT`/`UG_HUEU`/`UG_HUED`/`UG_VALU`/`UG_VALD` (formerly `RGB_TOG`/`RGB_MOD`/`RGB_HUI`/`RGB_HUD`/`RGB_VAI`/`RGB_VAD`).
* The Vial build has `VIAL_INSECURE = yes` set, so the keymap can be edited immediately with no unlock combo needed (remove this and switch to an unlock-combo scheme if security against loss/theft matters to you).
* **`NO_SUSPEND_POWER_DOWN` is set in `config.h`.** By default, QMK turns the backlight off the instant the host suspends the USB connection and snaps it back to full brightness — with no fade — the instant it wakes up; if the host's USB power management periodically suspends an idle keyboard, this shows up as a visible backlight flash every few minutes. Since this is a wired keyboard with no battery to save, there's no real benefit to powering down on suspend, so this behavior is disabled outright.
* RP2040 has plenty of flash (2MB) and EEPROM headroom (via wear-leveling), so Vial's Tap Dance, Combos, Key Overrides, and Mouse Keys can all stay enabled without running into space constraints (unlike the ATmega32U4-based V213 board, where several of these had to be disabled to fit).

Make example for this keyboard (after setting up your build environment):

    make krsplit/p213:default

Flashing: RP2040 doesn't need QMK Toolbox. After entering the bootloader, the board mounts as an `RPI-RP2` drive — just drag and drop the built `.uf2` file onto it, and it reboots and applies the firmware automatically.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: double-tap the reset button quickly — the standard way to enter the RP2040's UF2 bootloader
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
