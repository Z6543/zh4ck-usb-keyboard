// SPDX-FileCopyrightText: 2025 Orange-Murker
// SPDX-License-Identifier: MIT

#pragma once

#include "freertos/idf_additions.h"
#include "indev/lv_indev.h"

// Custom key codes for function keys not defined in lv_key_t
#define ZH4CK_KEY_F3 0xF3
#define ZH4CK_KEY_F5 0xF5
#define ZH4CK_KEY_F6 0xF6

// Initialize the USB keyboard LVGL input device.
// Creates an LVGL keypad indev that reads BSP input events from the
// provided queue and maps them to LVGL key events.
// Also creates and sets a default LVGL input group.
// Returns the created LVGL input device.
lv_indev_t* zh4ck_usb_keyboard_init(QueueHandle_t input_event_queue);
