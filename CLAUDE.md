# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an ESP-IDF component library that bridges hardware USB keyboard input events to LVGL (Light and Versatile Graphics Library) UI framework key events. It is designed for ESP32-based badge devices using the `badge-bsp` Board Support Package.

## Build System

This is an ESP-IDF component, not a standalone project. It must be included in a parent ESP-IDF project:

```cmake
# Parent project CMakeLists.txt adds this as a component
idf_component_register(SRCS "..." REQUIRES zh4ck-usb-keyboard)
```

Build via the parent project:
```bash
idf.py build
idf.py flash monitor
```

There are no standalone tests or linters configured in this repo — validation happens through integration with a parent ESP-IDF project.

## Architecture

Single-purpose component with two entry points:

**`zh4ck_usb_keyboard_init(input_queue)`** — Public init function. Stores the FreeRTOS queue handle, registers an LVGL keypad input device, creates a default LVGL input group, and returns the `lv_indev_t*`.

**`read_keyboard()`** — LVGL input device callback (called by LVGL's input subsystem). Non-blocking: reads from the FreeRTOS queue with timeout 0. Sets `continue_reading = true` when multiple events are pending to let LVGL drain the queue efficiently.

### Key mapping

Converts `bsp_input_event_t` events to LVGL key codes:

| Hardware input | LVGL key |
|---|---|
| Arrow keys | `LV_KEY_UP/DOWN/LEFT/RIGHT` |
| Return / Gamepad A | `LV_KEY_ENTER` |
| Esc / Gamepad B | `LV_KEY_ESC` |
| Tab | `LV_KEY_NEXT` |
| Shift+Tab | `LV_KEY_PREV` |
| Backspace | `LV_KEY_BACKSPACE` |
| F1 / F2 | `LV_KEY_HOME` / `LV_KEY_END` |
| F3, F5, F6 | `ZH4CK_KEY_F3/F5/F6` (custom, defined in header) |

Unhandled events are logged at `ESP_LOGW` level with the scancode.

## Dependencies

- `badge-bsp` — provides `bsp_input_event_t` and input queue
- `lvgl` — provides `lv_indev_t`, key codes, input groups
