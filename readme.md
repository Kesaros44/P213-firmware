# P213-firmware

QMK firmware for the P213 split keyboard — a wired, RP2040-based split board with a rotary encoder and analog joystick on the right half for cursor and scroll control.

* Keyboard Maintainer: [AJG](https://github.com/Kesaros44)
* Hardware Supported: krsplit P213 split keyboard (RP2040), wired only
* Hardware Availability: personal/custom build

![P213 완성 사진](images/p213-assembled.png)

## Hardware

- **MCU:** RP2040, UF2 bootloader
- **Matrix:** 12 rows × 10 columns (6 active rows per half), `COL2ROW` diode direction
- **Split communication:** soft serial, auto master/handedness detection
- **Encoder + analog joystick:** right half only
- **Backlight:** PWM, 7 levels, breathing animation
- **RGB underglow:** WS2812 × 4 (2 per half)
- Wired USB-C only — no battery, no sleep mode

## Keymap

Two build variants:

- **`keymaps/default`** — plain QMK, no VIA/Vial.
- **`keymaps/vial`** — VIA/Vial-enabled (needs the `vial-kb/vial-qmk` fork). `VIAL_INSECURE` is on, so it unlocks for editing with no combo needed.

4 layers: Windows base, Mac base (Cmd/Opt swapped), Fn (backlight/RGB control), and an auto-triggered mouse layer that activates when the joystick moves and reverts after 350ms idle. Built-in autocorrect (QMK community dictionary) is also enabled.

## Building

GitHub Actions builds both keymaps on every push — grab the `.uf2` files from the workflow run's artifacts.

Local build (see `BUILD_AND_VIAL_GUIDE.txt` for the exact file setup):

```sh
# default keymap — mainline QMK
make krsplit/p213:default

# vial keymap — needs the vial-qmk fork instead
make krsplit/p213:vial
```

## Flashing

RP2040 doesn't need QMK Toolbox. Enter the bootloader (double-tap reset, or hold the top-left key while plugging in), then drag the built `.uf2` file onto the `RPI-RP2` drive — it flashes and reboots automatically.

---

# P213-firmware (한국어)

P213 스플릿 키보드용 QMK 펌웨어입니다 — RP2040 기반 유선 스플릿 보드로, 오른쪽 half에 로터리 인코더와 아날로그 조이스틱을 탑재해 커서·스크롤을 조작할 수 있습니다.

* 키보드 관리자: [AJG](https://github.com/Kesaros44)
* 지원 하드웨어: krsplit P213 스플릿 키보드 (RP2040), 유선 전용
* 하드웨어 구입처: 개인/커스텀 제작

## 하드웨어

- **MCU:** RP2040, UF2 부트로더
- **매트릭스:** 12행 × 10열(half당 6행 사용), `COL2ROW` 다이오드 방향
- **스플릿 통신:** 소프트 시리얼, 마스터/좌우 자동 판별
- **인코더 + 아날로그 조이스틱:** 오른쪽 half 전용
- **백라이트:** PWM 7단계, 브리딩 애니메이션
- **RGB 언더글로우:** WS2812 4개(half당 2개)
- 유선 USB-C 전용 — 배터리, Sleep 모드 없음

## 키맵

두 가지 빌드가 있습니다:

- **`keymaps/default`** — 순정 QMK, VIA/Vial 없음.
- **`keymaps/vial`** — VIA/Vial 지원(`vial-kb/vial-qmk` 포크 필요). `VIAL_INSECURE`가 켜져 있어 언락 콤보 없이 바로 편집 가능.

4개 레이어: Windows 베이스, Mac 베이스(Cmd/Opt 교체), Fn(백라이트/RGB 제어), 조이스틱을 움직이면 자동 진입하고 350ms 유휴 시 자동 복귀하는 마우스 레이어. QMK 커뮤니티 사전 기반 자동 오타 수정 기능도 내장되어 있습니다.

## 빌드

GitHub Actions가 push마다 두 키맵을 모두 빌드합니다 — 워크플로우 아티팩트에서 `.uf2` 파일을 받으면 됩니다.

로컬 빌드(정확한 파일 구성은 `BUILD_AND_VIAL_GUIDE.txt` 참고):

```sh
# default 키맵 — mainline QMK
make krsplit/p213:default

# vial 키맵 — vial-qmk 포크 필요
make krsplit/p213:vial
```

## 플래싱

RP2040은 QMK Toolbox가 필요 없습니다. 부트로더로 진입한 뒤(리셋 버튼 더블탭, 또는 좌상단 키를 누른 채 연결), 빌드된 `.uf2` 파일을 `RPI-RP2` 드라이브에 드래그하면 자동으로 플래시되고 재부팅됩니다.
