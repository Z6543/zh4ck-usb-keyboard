// SPDX-FileCopyrightText: 2025 Orange-Murker
// SPDX-License-Identifier: MIT

#include "zh4ck_usb_keyboard.h"
#include "bsp/input.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "indev/lv_indev.h"

#include <stdint.h>

static char const TAG[] = "zh4ck-usb-keyboard";

static QueueHandle_t input_queue = NULL;

static void read_keyboard(lv_indev_t* indev, lv_indev_data_t* data) {
    bsp_input_event_t event;
    UBaseType_t messages_waiting = uxQueueMessagesWaiting(input_queue);

    if (messages_waiting > 1) {
        data->continue_reading = true;
    }

    if (messages_waiting < 1) {
        return;
    }

    if (xQueueReceive(input_queue, &event, 0) != pdTRUE) {
        return;
    }

    switch (event.type) {
        case INPUT_EVENT_TYPE_NAVIGATION:
            switch (event.args_navigation.key) {
                case BSP_INPUT_NAVIGATION_KEY_UP:
                    data->key   = LV_KEY_UP;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_DOWN:
                    data->key   = LV_KEY_DOWN;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_LEFT:
                    data->key   = LV_KEY_LEFT;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_RIGHT:
                    data->key   = LV_KEY_RIGHT;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_RETURN:
                    data->key   = LV_KEY_ENTER;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_GAMEPAD_A:
                    data->key   = LV_KEY_ENTER;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_ESC:
                    data->key   = LV_KEY_ESC;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_GAMEPAD_B:
                    data->key   = LV_KEY_ESC;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_TAB:
                    if (event.args_navigation.modifiers
                        & BSP_INPUT_MODIFIER_SHIFT) {
                        data->key = LV_KEY_PREV;
                    } else {
                        data->key = LV_KEY_NEXT;
                    }
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_BACKSPACE:
                    data->key   = LV_KEY_BACKSPACE;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_F1:
                    data->key   = LV_KEY_HOME;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_F2:
                    data->key   = LV_KEY_END;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_F3:
                    data->key   = ZH4CK_KEY_F3;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_F5:
                    data->key   = ZH4CK_KEY_F5;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_F6:
                    data->key   = ZH4CK_KEY_F6;
                    data->state = event.args_navigation.state;
                    break;
                case BSP_INPUT_NAVIGATION_KEY_SPACE_L:
                case BSP_INPUT_NAVIGATION_KEY_SPACE_M:
                case BSP_INPUT_NAVIGATION_KEY_SPACE_R:
                case BSP_INPUT_NAVIGATION_KEY_F4:
                case BSP_INPUT_NAVIGATION_KEY_VOLUME_UP:
                case BSP_INPUT_NAVIGATION_KEY_VOLUME_DOWN:
                case BSP_INPUT_NAVIGATION_KEY_SUPER:
                    break;
                default:
                    ESP_LOGW(TAG, "Unhandled nav event");
                    break;
            }
            break;
        case INPUT_EVENT_TYPE_KEYBOARD:
            data->state = LV_INDEV_STATE_PRESSED;
            data->key = *((uint32_t*)event.args_keyboard.utf8);
            break;
        case INPUT_EVENT_TYPE_ACTION:
            break;
        case INPUT_EVENT_TYPE_SCANCODE:
            break;
        default:
            ESP_LOGW(TAG, "Unhandled input event type %u", event.type);
            break;
    }
}

lv_indev_t* zh4ck_usb_keyboard_init(QueueHandle_t input_event_queue) {
    input_queue = input_event_queue;

    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev, read_keyboard);

    lv_group_t* group = lv_group_create();
    lv_indev_set_group(indev, group);
    lv_group_set_default(group);

    return indev;
}
