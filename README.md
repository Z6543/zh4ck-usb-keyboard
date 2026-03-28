# zh4ck-usb-keyboard

ESP-IDF component that maps USB keyboard input events from the `badge-bsp` Board Support Package to LVGL keypad input device events.

## Usage

Add this repository as a component in your ESP-IDF project, then initialize it by passing the BSP input event queue:

```c
#include "zh4ck_usb_keyboard.h"

QueueHandle_t input_queue = /* queue from badge-bsp */;
lv_indev_t *kb = zh4ck_usb_keyboard_init(input_queue);
```

`zh4ck_usb_keyboard_init` registers an LVGL keypad input device, creates a default input group, and returns the `lv_indev_t*`. Call it after `lv_init()` and before starting the LVGL task.

## Key mapping

| Key | LVGL event |
|-----|-----------|
| Arrow keys | `LV_KEY_UP/DOWN/LEFT/RIGHT` |
| Return, Gamepad A | `LV_KEY_ENTER` |
| Esc, Gamepad B | `LV_KEY_ESC` |
| Tab | `LV_KEY_NEXT` |
| Shift+Tab | `LV_KEY_PREV` |
| Backspace | `LV_KEY_BACKSPACE` |
| F1 | `LV_KEY_HOME` |
| F2 | `LV_KEY_END` |
| F3 | `ZH4CK_KEY_F3` (0xF3) |
| F5 | `ZH4CK_KEY_F5` (0xF5) |
| F6 | `ZH4CK_KEY_F6` (0xF6) |
| Keyboard input | UTF-8 character as `uint32_t` |

Space (L/M/R), F4, volume keys, and Super are silently ignored. All other unhandled events are logged at `LOGW` level.

## License

MIT — © 2025 Orange-Murker
