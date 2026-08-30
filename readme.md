# krsplit P213

krsplit 시리즈의 RP2040 기반 무선(유선 USB-C) 스플릿 커스텀 키보드입니다.
ANSI 107키 배열을 기준으로 설계된 컬럼/로우 스태거드 스플릿 키보드로,
오른쪽 반쪽에 로터리 인코더 1개와 아날로그 조이스틱(포인팅 디바이스)이
달려 있어 별도의 마우스 없이 커서 이동/스크롤이 가능합니다.

* Keyboard Maintainer: [kesaros44](https://github.com/kesaros44)
* Hardware Supported: RP2040 (Raspberry Pi RP2040), 스플릿 PCB 좌/우 각 1장
* Hardware Availability: 개인 제작 커스텀 PCB (공개 배포처 없음)

## 하드웨어 사양

* MCU: RP2040 (ARM Cortex-M0+, 부트로더: UF2)
* 매트릭스: 12행 × 10열 (좌/우 각 6행씩 사용), 다이오드 방향 COL2ROW
* 스플릿 통신: soft serial (GP1), `SPLIT_USB_DETECT`로 좌/우 어느 쪽에
  USB를 꽂아도 자동으로 마스터가 됨, `SPLIT_HAND_PIN`(GP3)으로 좌/우 판별
* 로터리 인코더: 오른쪽 반쪽에 1개 (GP16/GP17). 왼쪽은 물리적으로 없고
  자리만 잡아둔 상태(`NO_PIN`) — 좌우 인덱스를 맞추기 위한 설계
* 백라이트: PWM 구동(GP28), 7단계 밝기, 브리딩 효과 지원(6초 주기)
* RGB 언더글로우: WS2812 4구(좌/우 2구씩, GP29)
* 포인팅 디바이스: 아날로그 조이스틱(X: GP26, Y: GP27) — 조이스틱을
  움직이면 자동으로 마우스 전용 레이어로 전환됨(350ms 후 자동 복귀),
  일정 각도 이상 기울이면 스크롤로 전환되는 커스텀 로직 포함
* 부트로더 진입: 리셋 버튼 더블탭(RP2040 표준 UF2 방식), GP17 LED로
  부트로더 진입 표시

## 소프트웨어 기능

* 레이어 4개: Windows 기준 레이어, Mac 기준 레이어(Cmd/Opt 위치 교체),
  FN 레이어(백라이트/RGB 컨트롤), Mouse 레이어(조이스틱 사용 시 자동 진입)
* 자동 오타 수정(Autocorrection) 내장 (QMK 커뮤니티 기능, 자체 사전 데이터 포함)
* 두 가지 빌드로 나뉨 — 자세한 빌드 명령어는 `BUILD_AND_VIAL_GUIDE.txt` 참고
  * `keymaps/default` : 순정 QMK 빌드 (VIA/Vial 없음)
  * `keymaps/vial` : [Vial](https://get.vial.today/)로 키맵/매크로/콤보를
    펌웨어 재컴파일 없이 실시간으로 편집 가능한 빌드
    (반드시 [vial-kb/vial-qmk](https://github.com/vial-kb/vial-qmk) 포크로
    빌드해야 하며, mainline QMK로는 빌드되지 않음)

## 주요사항 (직접 겪은 이슈들)

* **키보드 정의 파일은 `keyboard.json`이어야 함.** 예전 이름인
  `info.json`은 최신 QMK가 아예 인식하지 못함(빌드 자체가 무시됨).
* **키보드 레벨 C 소스 파일은 반드시 폴더 이름과 같아야 자동으로
  컴파일에 포함됨.** 이 보드는 `krsplit.c`가 아니라 `p213.c`여야
  QMK 빌드 시스템이 이 파일을 실제로 컴파일함 — 파일명이 다르면
  겉으로는 빌드가 성공해도 이 파일 안의 코드(자동 오타수정 연결,
  RGB 상태 동기화, EEPROM 초기화 로직 등)가 조용히 통째로 빠진 채로
  펌웨어가 만들어짐. 반드시 파일명을 확인할 것.
* 인코더 핀은 최신 이름(`ENCODER_A_PINS`/`ENCODER_B_PINS`(`_RIGHT`))을
  사용함 — 예전 이름(`ENCODERS_PAD_A`/`ENCODERS_PAD_B`)을 쓰면 최신
  QMK의 자동 생성 코드와 이름이 겹쳐서 컴파일 에러가 남.
* 키맵에서 마우스 버튼/언더글로우 키코드는 최신 이름을 사용함:
  `MS_BTN1`/`MS_BTN2`(구: `KC_MS_BTN1`/`KC_MS_BTN2`),
  `UG_TOGG`/`UG_NEXT`/`UG_HUEU`/`UG_HUED`/`UG_VALU`/`UG_VALD`
  (구: `RGB_TOG`/`RGB_MOD`/`RGB_HUI`/`RGB_HUD`/`RGB_VAI`/`RGB_VAD`).
* Vial 빌드는 `VIAL_INSECURE = yes`로 설정되어 있어 언락 콤보 없이
  바로 키맵을 편집할 수 있음(분실/도난 시 보안이 필요하면 이 옵션을
  빼고 언락 콤보 방식으로 바꿀 것).
* RP2040은 플래시(2MB)/EEPROM(wear-leveling) 여유가 충분해서 Vial의
  Tap Dance/Combo/Key Override/Mousekey를 전부 켜둔 상태로도 문제없음.

Make example for this keyboard (after setting up your build environment):

    make krsplit/p213:default

Flashing: RP2040은 QMK Toolbox가 필요 없습니다. 부트로더 진입 후
`RPI-RP2` 드라이브로 마운트되면 만들어진 `.uf2` 파일을 그대로
끌어다 놓으면 자동으로 재부팅되며 적용됩니다.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: 리셋 버튼을 빠르게 두 번 누름(더블탭) — RP2040 표준 UF2 부트로더 진입 방식
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
